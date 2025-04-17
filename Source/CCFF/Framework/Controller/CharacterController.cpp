#include "CharacterController.h"
#include "Blueprint/UserWidget.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/UI/TogglePauseWidget.h"
#include "Framework/GameMode/TrainingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Base/BaseCharacter.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "Framework/GameMode/ArenaGameMode.h"
#include "Engine/World.h"
#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Camera/CameraActor.h"
#include "Framework/GameState/ArenaGameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Manager/CustomizationManager.h"
#include "TimerManager.h" 
#include "Framework/HUD/ArenaModeHUD.h"


ACharacterController::ACharacterController()
	: DefaultMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	PauseWidget(nullptr),
	bIsPause(false)
{
	AttackAction.SetNum(8);
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		if (UCCFFGameInstance* CCFFGameInstance = Cast<UCCFFGameInstance>(GetGameInstance()))
		{
			const FString NickName = CCFFGameInstance->GetNickname();
			//UE_LOG(LogTemp, Log, TEXT("Client BeginPlay: Sending Nickname = '%s'"), *NickName);
			ServerSetNickname(NickName);
			FName SelectedCharacterID = CCFFGameInstance->GetSelectedCharacterID();
			int32 SelectedPresetIndex = CCFFGameInstance->GetLobbyPresetIndex();

			UCustomizationManager* CustomizationManager = GetGameInstance()->GetSubsystem<UCustomizationManager>();
			if (CustomizationManager)
			{
				const auto Presets = CustomizationManager->GetCharacterCustomizationPresets();
				Server_ReadyToSpawn(SelectedCharacterID, SelectedPresetIndex, Presets);
			}

			CCFFGameInstance->PlayBGMForCurrentMap();
		}
	}

	bShowMouseCursor = true;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);
}

void ACharacterController::TogglePause()
{
	bIsPause = !bIsPause;
	//SetPause(bIsPause);

	ABaseInGameHUD* MyInGameHUD = Cast<ABaseInGameHUD>(GetHUD());
	if (IsValid(MyInGameHUD))
	{
		//UE_LOG(LogTemp, Log, TEXT("+++++++++++++++++++++++++++++ [CharacterController] Valid MyBaseInGameHDU"));
		UUserWidget* ToggleWidget = MyInGameHUD->GetTogglePauseWidget();
		if (ToggleWidget != nullptr)
		{
			if (ToggleWidget->GetVisibility() == ESlateVisibility::Collapsed)
				ToggleWidget->SetVisibility(ESlateVisibility::Visible);
			else
				ToggleWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (bIsPause)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
	else
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
	}
}

void ACharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("TogglePause"), EInputEvent::IE_Pressed, this, &ACharacterController::TogglePause);
}

void ACharacterController::ServerReturnToLobby_Implementation()
{
	GetWorld()->ServerTravel(TEXT("/Game/CCFF/Maps/LobbyMap?Listen"));
}

bool ACharacterController::ServerReturnToLobby_Validate()
{
	return true;
}

void ACharacterController::ServerReturnToMainMenu_Implementation()
{
	GetWorld()->ServerTravel(TEXT("/Game/CCFF/Maps/MainMenuMap?Listen"));
}

bool ACharacterController::ServerReturnToMainMenu_Validate()
{
	return true;
}

void ACharacterController::ServerSetNickname_Implementation(const FString& InNickname)
{
	if (AArenaPlayerState* ArenaPlayerState = GetPlayerState<AArenaPlayerState>())
	{
		ArenaPlayerState->SetPlayerNickname(InNickname);
		ArenaPlayerState->SetPlayerName(InNickname);

		//UE_LOG(LogTemp, Log, TEXT("ServerSetNickname: Received Nickname = '%s'"), *InNickname);

	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("ServerSetNickname: PlayerState invalid"));
	}
}

bool ACharacterController::ServerSetNickname_Validate(const FString& InNickname)
{
	return true;
}

void ACharacterController::Server_ReadyToSpawn_Implementation(FName SelectedID, int32 PresetIndex, const TArray<FCharacterCustomizationPreset>& Presets)
{
	if (AArenaPlayerState* PS = GetPlayerState<AArenaPlayerState>())
	{
		PS->SetSelectedCharacterID(SelectedID);
		PS->SetSelectedPresetIndex(PresetIndex);
		PS->SetCharacterCustomizationPresets(Presets);

		if (AArenaGameMode* GM = GetWorld()->GetAuthGameMode<AArenaGameMode>())
		{
			GM->SpawnPlayer(this);
		}
	}
}

// TODO :: Fix Respawn SpectatorCam
void ACharacterController::NotifyPawnDeath()
{
	if (!HasAuthority()) { return; }

	AArenaPlayerState* ArenaPlayerState = GetPlayerState<AArenaPlayerState>();
	if (!IsValid(ArenaPlayerState)) { return; }

	AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(ArenaGameMode)) { return; }

	// Posses Spectator Camera
	if (AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GetWorld()->GetGameState()))
	{
		ACameraActor* SpectatorCam = ArenaGameState->GetSpectatorCamera();
		if (IsValid(SpectatorCam))
		{
			ClientSpectateCamera(SpectatorCam);
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("++++++++++++++++ [CharacterController] SpectatorCamera is Not! Valid!!"));
		}

		if (IsLocalController())
		{
			ClientSpectateCamera(SpectatorCam);
		}
	}

	// Respawn
	if (ArenaGameMode->SelectedArenaSubMode == EArenaSubMode::DeathMatch)
	{
		//UE_LOG(LogTemp, Log, TEXT("+++++++++++++++++++++++++++++++ [CharacterController] DeathMatch Mode"));
		FTimerHandle RespawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, [this, ArenaGameMode]()
			{
				ArenaGameMode->SpawnPlayer(this);
			}, 5.0f, false);
	}
	else if (ArenaGameMode->SelectedArenaSubMode == EArenaSubMode::Elimination)
	{
		if (ArenaPlayerState->MaxLives >= 0)
		{
			ArenaPlayerState->MaxLives--;

			//UE_LOG(LogTemp, Log, TEXT("+++++++++++++++++++++++++++++++ [CharacterController] Elimination Mode, MaxLive is %d"), ArenaPlayerState->MaxLives);
			FTimerHandle RespawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, [this, ArenaGameMode]()
				{
					ArenaGameMode->SpawnPlayer(this);
				}, 5.0f, false);
		}
		else
		{
			if (AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GetWorld()->GetGameState()))
			{
				float SurvivalTime = ArenaGameState->GetRoundStartTime() - ArenaGameState->GetRemainingTime();
				ArenaPlayerState->SetSurvivalTime(SurvivalTime);
				//UE_LOG(LogTemp, Log, TEXT("++++++++++++++++++++++++++++++++++++++++ Survival Time : %.2f"), SurvivalTime);
			}
		}
	}
}

void ACharacterController::ClientSpectateCamera_Implementation(ACameraActor* SpectatorCam)
{
	//UE_LOG(LogTemp, Warning,TEXT("[ClientSpectateCamera] called on %s (NetMode=%d), Cam ptr=%p"),*GetName(), (int)GetNetMode(), SpectatorCam);
	if (!SpectatorCam)
	{
		//UE_LOG(LogTemp, Warning, TEXT("++++++++++++++++++++++++++++++++ [ClientSpectateCamera] SpectatorCam is nullptr"));
		return;
	}

	DisableInput(this);
	FlushPressedKeys();
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);

	UnPossess();
	ChangeState(NAME_Spectating);
	SetViewTargetWithBlend(SpectatorCam, 0.f);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this, SpectatorCam]()
		{
			SetViewTargetWithBlend(SpectatorCam, 0.f);
		}, 0.1f, false);
}

