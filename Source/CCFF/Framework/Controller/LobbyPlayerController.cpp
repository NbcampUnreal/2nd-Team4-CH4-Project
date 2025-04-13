#include "Framework/Controller/LobbyPlayerController.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/GameMode/LobbyGameMode.h"
#include "Framework/GameState/LobbyGameState.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Items/Manager/CustomizationManager.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Framework/UI/LobbyWidget.h"
#include "Framework/UI/CountdownWidget.h"
#include "Framework/UI/CharacterSelectWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	HandleLocalSetup();

	UCCFFGameInstance* GameInstance = GetGameInstance<UCCFFGameInstance>();
	if (GameInstance)
	{
		ServerSetNickname(GameInstance->GetNickname());
	}

	if (CharacterSelectWidgetClass && !CharacterSelectWidgetInstance)
	{
		CharacterSelectWidgetInstance = CreateWidget<UCharacterSelectWidget>(this, CharacterSelectWidgetClass);
		if (CharacterSelectWidgetInstance)
		{
			CharacterSelectWidgetInstance->OnCharacterSelected.BindUObject(
				this,
				&ALobbyPlayerController::HandleCharacterSelectedFromUI
			);

			CharacterSelectWidgetInstance->AddToViewport();
			CacheCharacterIDList();
		}
	}

	SetupEnhancedInput();
}

void ALobbyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsLocalController()) return;

	SetupEnhancedInput();

	if (CharacterIDList.IsEmpty())
	{
		CacheCharacterIDList();
	}
}

void ALobbyPlayerController::SetupEnhancedInput()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (CharacterSelectInputContext)
		{
			Subsystem->AddMappingContext(CharacterSelectInputContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_NavigateHorizontal)
		{
			EnhancedInput->BindAction(IA_NavigateHorizontal, ETriggerEvent::Triggered, this, &ALobbyPlayerController::HandleHorizontalInput);
		}

		if (IA_NavigateVertical)
		{
			EnhancedInput->BindAction(IA_NavigateVertical, ETriggerEvent::Triggered, this, &ALobbyPlayerController::HandleVerticalInput);
		}
	}
}

void ALobbyPlayerController::CacheCharacterIDList()
{
	if (CharacterSelectWidgetInstance && CharacterIDList.IsEmpty())
	{
		CharacterIDList = CharacterSelectWidgetInstance->GetCharacterIDList();
	}
}

void ALobbyPlayerController::HandleLocalSetup()
{
	ShowLobbyUI();
}

void ALobbyPlayerController::SetLobbyCameraView()
{
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LobbyCam"), FoundCameras);

	if (FoundCameras.Num() > 0)
	{
		SetViewTargetWithBlend(FoundCameras[0], 0.5f);
	}
}

void ALobbyPlayerController::SetAllowSoloStart(int32 Allow)
{
	ServerSetAllowSoloStart(Allow);
}

void ALobbyPlayerController::ServerSetAllowSoloStart_Implementation(int32 Allow)
{
	if (ALobbyGameMode* GM = GetWorld()->GetAuthGameMode<ALobbyGameMode>())
	{
		GM->bAllowSoloStart = (Allow > 0);
		UE_LOG(LogTemp, Warning, TEXT("[Server] SetAllowSoloStart = %s"), Allow > 0 ? TEXT("true") : TEXT("false"));

		if (ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>())
		{
			GS->UpdateAllowStartGame();
		}
	}
}

void ALobbyPlayerController::ShowLobbyUI()
{
	if (LobbyWidgetClass && !LobbyWidgetInstance)
	{
		LobbyWidgetInstance = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		if (LobbyWidgetInstance)
		{
			LobbyWidgetInstance->AddToViewport();
		}
	}
}

void ALobbyPlayerController::ServerSetNickname_Implementation(const FString& InNickname)
{
	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState) == true)
	{
		LobbyPlayerState->SetPlayerNickname(InNickname);
	}
}

void ALobbyPlayerController::ServerToggleReady_Implementation()
{
	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState) == true)
	{
		bool bNewState = !LobbyPlayerState->IsReady();
		LobbyPlayerState->SetReady(bNewState);

		ALobbyGameMode* LobbyGameMode = GetWorld()->GetAuthGameMode<ALobbyGameMode>();
		if (IsValid(LobbyGameMode) == true)
		{
			LobbyGameMode->NotifyPlayerReadyStatusChanged();
		}
	}
}

void ALobbyPlayerController::UpdateCountdownWidget_Implementation(int32 NewTime)
{
	if (!CountdownWidgetInstance && CountdownWidgetClass)
	{
		CountdownWidgetInstance = CreateWidget<UCountdownWidget>(this, CountdownWidgetClass);
		if (CountdownWidgetInstance)
		{
			CountdownWidgetInstance->AddToViewport();
		}
	}

	if (CountdownWidgetInstance)
	{
		if (NewTime >= 0)
		{
			CountdownWidgetInstance->SetCountdownText(FString::FromInt(NewTime));
		}
		else
		{
			CountdownWidgetInstance->RemoveFromParent();
			CountdownWidgetInstance = nullptr;
		}
	}
}

void ALobbyPlayerController::ClientTeardownCountdown_Implementation()
{
	if (CountdownWidgetInstance)
	{
		CountdownWidgetInstance->RemoveFromParent();
		CountdownWidgetInstance = nullptr;
	}
}

void ALobbyPlayerController::HandleCharacterSelectedFromUI(FName CharacterID)
{
	ServerSetCharacterID(CharacterID);

	int32 Index = CharacterIDList.IndexOfByKey(CharacterID);
	if (Index != INDEX_NONE)
	{
		CurrentCharacterIndex = Index;
	}
}

void ALobbyPlayerController::HandleHorizontalInput(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	if (CharacterIDList.Num() == 0) return;

	if (Direction > 0.1f)
	{
		CurrentCharacterIndex = (CurrentCharacterIndex + 1) % CharacterIDList.Num();
	}
	else if (Direction < -0.1f)
	{
		CurrentCharacterIndex = (CurrentCharacterIndex - 1 + CharacterIDList.Num()) % CharacterIDList.Num();
	}

	const FName SelectedID = CharacterIDList[CurrentCharacterIndex];
	ServerSetCharacterID(SelectedID);
}

void ALobbyPlayerController::HandleVerticalInput(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	if (Direction > 0.1f)
	{
		// ↑ 위쪽 → 프리셋 업
	}
	else if (Direction < -0.1f)
	{
		// ↓ 아래쪽 → 프리셋 다운
	}
}

void ALobbyPlayerController::ServerSetCharacterID_Implementation(FName CharacterID)
{
	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState))
	{
		LobbyPlayerState->SetCharacterID(CharacterID);
	}
}

void ALobbyPlayerController::SetCustomizationPresets()
{
	if (IsLocalController())
	{
		ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
		if (IsValid(LobbyPlayerState))
		{
			UCustomizationManager* CustomizationManager = GetGameInstance()->GetSubsystem<UCustomizationManager>();
			if (IsValid(CustomizationManager))
			{
				TArray<FCharacterCustomizationPreset> Presets = CustomizationManager->GetCharacterCustomizationPresets();
				Server_SetPresetsToPlayerState(Presets);
			}
		}
	}
}

void ALobbyPlayerController::Server_SetPresetsToPlayerState_Implementation(const TArray<FCharacterCustomizationPreset>& ClientPresets)
{
	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState) == true)
	{
		LobbyPlayerState->SetCharacterCustomizationPresets(ClientPresets);

		// Just For Logging Delete Later
		UE_LOG(LogTemp, Log, TEXT("===== All Character Presets ====="));

		const UEnum* EnumPtr = StaticEnum<EItemSlot>();
		for (const FCharacterCustomizationPreset& CharPreset : ClientPresets)
		{
			UE_LOG(LogTemp, Log, TEXT("▶ Character ID: %s"), *CharPreset.CharacterID.ToString());

			for (const FCustomizationPreset& Preset : CharPreset.Presets)
			{
				UE_LOG(LogTemp, Log, TEXT("  - Preset Index: %d"), Preset.PresetIndex);

				for (const FEquippedItemData& Item : Preset.EquippedItems)
				{
					FString SlotName = EnumPtr
						? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Item.EquipSlot)).ToString()
						: TEXT("Unknown");

					UE_LOG(LogTemp, Log, TEXT("    - Slot: %s, ItemID: %s"), *SlotName, *Item.ItemID.ToString());
				}
			}
		}

		UE_LOG(LogTemp, Log, TEXT("=================================="));
	}
}

void ALobbyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerState called. PlayerState is now valid."));

	SetCustomizationPresets();
}