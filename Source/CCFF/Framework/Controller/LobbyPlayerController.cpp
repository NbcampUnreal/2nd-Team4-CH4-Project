#include "Framework/Controller/LobbyPlayerController.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/GameMode/LobbyGameMode.h"
#include "Framework/GameState/LobbyGameState.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Items/Manager/CustomizationManager.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Character/Lobby/LobbyPreviewPawn.h"
#include "Framework/UI/LobbyWidget.h"
#include "Framework/UI/CountdownWidget.h"
#include "Framework/UI/CharacterSelectWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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

	if (UCCFFGameInstance* CCFFGameInstance = GetGameInstance<UCCFFGameInstance>())
	{
		CCFFGameInstance->SetSelectedCharacterID(CharacterID);
	}

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
	UE_LOG(LogTemp, Log, TEXT("[HandleVerticalInput] Received input value: %f"), Direction);

	if (Direction > 0.1f)
	{
		int32 IndexDirection = 1;
		UE_LOG(LogTemp, Log, TEXT("[HandleVerticalInput] Sending IndexDirection: %d to server"), IndexDirection);
		Server_SwitchPresetIndex(IndexDirection);
	}
	else if (Direction < -0.1f)
	{
		int32 IndexDirection = -1;
		UE_LOG(LogTemp, Log, TEXT("[HandleVerticalInput] Sending IndexDirection: %d to server"), IndexDirection);
		Server_SwitchPresetIndex(IndexDirection);
	}
}


void ALobbyPlayerController::ServerSetCharacterID_Implementation(FName CharacterID)
{
	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState))
	{
		LobbyPlayerState->SetCharacterID(CharacterID);
	}

	CurrentPresetIndex = -1;
}

void ALobbyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerState called. PlayerState is now valid."));

	// Set Client's Customization Presets to Server PlayerState
	SetCustomizationPresets();
}

void ALobbyPlayerController::SetCustomizationPresets()
{
	// Get Client's Customization Presets
	if (IsLocalController())
	{
		ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
		if (IsValid(LobbyPlayerState))
		{
			UCustomizationManager* CustomizationManager = GetGameInstance()->GetSubsystem<UCustomizationManager>();
			if (IsValid(CustomizationManager))
			{
				TArray<FCharacterCustomizationPreset> Presets = CustomizationManager->GetCharacterCustomizationPresets();
				
				// Send Client's Customization Presets to Server
				Server_SetPresetsToPlayerState(Presets);
			}
		}
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetCustomizationPresets called on server. Now Returning."));
		return;
	}
}

void ALobbyPlayerController::Server_SetPresetsToPlayerState_Implementation(const TArray<FCharacterCustomizationPreset>& ClientPresets)
{
	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState) == true)
	{
		LobbyPlayerState->SetCharacterCustomizationPresets(ClientPresets);
	}
}

void ALobbyPlayerController::Server_SwitchPresetIndex_Implementation(int32 IndexDirection)
{
	if (IsLocalController()){return;}

	int32 OldIndex = CurrentPresetIndex;
	CurrentPresetIndex = (CurrentPresetIndex + IndexDirection + 3) % 3;

	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState))
	{
		FCustomizationPreset IndexedPreset = LobbyPlayerState->GetCustomizationPreset(LobbyPlayerState->GetCharacterID(), CurrentPresetIndex);

		UE_LOG(LogTemp, Log, TEXT("[Server] Calling RequestEquipPreset with PresetIndex = % d"), CurrentPresetIndex);
		RequestEquipPreset(IndexedPreset);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] LobbyPlayerState is invalid."));
	}
}


void ALobbyPlayerController::RequestEquipPreset(FCustomizationPreset Preset)
{
	UE_LOG(LogTemp, Log, TEXT("[Server] RequestEquipPreset called. PresetIndex = %d, EquippedItemsCount = %d"),
		Preset.PresetIndex,
		Preset.EquippedItems.Num());
	
	APawn* ServerPawn = GetPawn();
	if (!ServerPawn) {return;}

	ABasePreviewPawn* ServerPreviewPawn = Cast<ABasePreviewPawn>(ServerPawn);
	if (!ServerPreviewPawn)	{return;}

	UCharacterCustomizationComponent* CustomizationComponent = Cast<UCharacterCustomizationComponent>(
		ServerPreviewPawn->GetComponentByClass(UCharacterCustomizationComponent::StaticClass()));

	if (CustomizationComponent)
	{
		
		UE_LOG(LogTemp, Log, TEXT("[Server] CustomizationComponent found."));
		if (Preset.PresetIndex == -1)
		{
			CustomizationComponent->UnequipAllItems();
			UE_LOG(LogTemp, Log, TEXT("[Server] No Saved Index. Unequipped all items."));
		}

		else
		{
			CustomizationComponent->EquipPreset(Preset);
			UE_LOG(LogTemp, Log, TEXT("[Server] Equipped Preset with Index %d"), Preset.PresetIndex);
		}
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] CustomizationComponent not found on PreviewPawn."));
	}
}


void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyPlayerController, CurrentPresetIndex);
}