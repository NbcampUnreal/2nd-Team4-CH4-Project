#include "Framework/Controller/LobbyPlayerController.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/GameMode/LobbyGameMode.h"
#include "Framework/GameState/LobbyGameState.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Items/Manager/CustomizationManager.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Framework/UI/LobbyWidget.h"
#include "Framework/UI/CountdownWidget.h"
#include "Kismet/GameplayStatics.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	if (IsLocalController())
	{
		HandleLocalSetup();
		
		UCCFFGameInstance* GameInstance = GetGameInstance<UCCFFGameInstance>();
		if (GameInstance)
		{
			ServerSetNickname(GameInstance->GetNickname());
		}
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

void ALobbyPlayerController::SetCustomizationPresets()
{
	if (IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("This is a local controller."));

		ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
		if (IsValid(LobbyPlayerState))
		{
			UE_LOG(LogTemp, Warning, TEXT("LobbyPlayerState is valid."));

			UCustomizationManager* CustomizationManager = GetGameInstance()->GetSubsystem<UCustomizationManager>();
			if (IsValid(CustomizationManager))
			{
				UE_LOG(LogTemp, Warning, TEXT("CustomizationManager is valid. Retrieving presets."));

				TArray<FCharacterCustomizationPreset> Presets = CustomizationManager->GetCharacterCustomizationPresets();
				UE_LOG(LogTemp, Warning, TEXT("Sending %d presets to server."), Presets.Num());

				Server_SetPresetsToPlayerState(Presets);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("CustomizationManager is NOT valid."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LobbyPlayerState is NOT valid."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("This is NOT a local controller."));
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
			UE_LOG(LogTemp, Log, TEXT("¢º Character ID: %s"), *CharPreset.CharacterID.ToString());

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