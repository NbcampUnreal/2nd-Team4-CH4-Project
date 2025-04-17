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
#include "Framework/UI/CountdownWidget.h"


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


void ACharacterController::NotifyPawnDeath()
{
	if (!HasAuthority()) { return; }

	AArenaPlayerState* ArenaPlayerState = GetPlayerState<AArenaPlayerState>();
	if (!IsValid(ArenaPlayerState)) { return; }

	AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(ArenaGameMode)) { return; }

	UnPossess();
	DisableInput(this);
	FlushPressedKeys();
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);

	ArenaPlayerState->MaxLives--;

	// Posses Spectator Camera
	if (AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GetWorld()->GetGameState()))
	{
		FTimerHandle SpectateHandle;
		ACameraActor* SpectatorCam = ArenaGameState->GetSpectatorCamera();
		if (IsValid(SpectatorCam))
		{
			GetWorld()->GetTimerManager().SetTimer(SpectateHandle, [this, SpectatorCam]()
				{
					FViewTargetTransitionParams Params;
					Params.BlendTime = 0.f;
					Cast<APlayerController>(this)
						->ClientSetViewTarget(SpectatorCam, Params);
				}, 0.2f, false);
		}

		if (IsLocalController())
		{
			GetWorld()->GetTimerManager().SetTimer(SpectateHandle, [this, SpectatorCam]()
				{
					FViewTargetTransitionParams Params;
					Params.BlendTime = 0.f;
					Cast<APlayerController>(this)
						->ClientSetViewTarget(SpectatorCam, Params);
				}, 0.2f, false);
		}
	}

	// Respawn
	AArenaPlayerState* PS = GetPlayerState<AArenaPlayerState>();
	const bool bHasLives = PS && PS->MaxLives > 0;

	if (ArenaGameMode->SelectedArenaSubMode == EArenaSubMode::DeathMatch)
	{
		ClientStartRespawnCountdown();
		FTimerHandle RespawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimerHandle,
			[this, ArenaGameMode]()
			{
				ArenaGameMode->SpawnPlayer(this);
			},
			5.0f,
			false
		);
	}
	else if (ArenaGameMode->SelectedArenaSubMode == EArenaSubMode::Elimination)
	{
		if (bHasLives)
		{
			ClientStartRespawnCountdown();
			FTimerHandle RespawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				RespawnTimerHandle,
				[this, ArenaGameMode]()
				{
					ArenaGameMode->SpawnPlayer(this);
				},
				5.0f,
				false
			);
		}
		else
		{
			if (AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GetWorld()->GetGameState()))
			{
				float SurvivalTime = ArenaGameState->GetRoundStartTime() - ArenaGameState->GetRemainingTime();
				ArenaPlayerState->SetSurvivalTime(SurvivalTime);
				ClientShowDieMessage();
			}
		}
	}
}

void ACharacterController::ClientStartRespawnCountdown_Implementation()
{
	if (!IsLocalController())
		return;

	if (AArenaModeHUD* ArenaModeHUD = Cast<AArenaModeHUD>(GetHUD()))
	{
		RespawnCountdownValue = 5;
		ArenaModeHUD->ShowCountdownWidget();
		ArenaModeHUD->UpdateCountdownText(FString::FromInt(RespawnCountdownValue));

		GetWorld()->GetTimerManager().SetTimer(
			RespawnCountdownTimerHandle,
			this,
			&ACharacterController::UpdateRespawnCountdown,
			1.0f,
			true
		);
	}
}

void ACharacterController::ClientShowDieMessage_Implementation()
{
	if (!IsLocalController()) return;

	if (AArenaModeHUD* HUD = Cast<AArenaModeHUD>(GetHUD()))
	{
		HUD->ShowCountdownWidget();
		HUD->UpdateCountdownText(TEXT("DIE!"));

		FTimerHandle HideHandle;
		GetWorldTimerManager().SetTimer(HideHandle, [HUD]()
			{
				HUD->HideCountdownWidget();
			}, 2.0f, false);
	}
}

void ACharacterController::UpdateRespawnCountdown()
{
	AArenaModeHUD* ArenaModeHUD = Cast<AArenaModeHUD>(GetHUD());
	if (!ArenaModeHUD)
	{
		GetWorld()->GetTimerManager().ClearTimer(RespawnCountdownTimerHandle);
		return;
	}

	RespawnCountdownValue--;

	if (RespawnCountdownValue > 0)
	{
		ArenaModeHUD->UpdateCountdownText(FString::FromInt(RespawnCountdownValue));
	}
	else
	{
		AArenaPlayerState* PS = GetPlayerState<AArenaPlayerState>();
		bool bHasLives = PS && PS->MaxLives > 0;

		ArenaModeHUD->UpdateCountdownText(TEXT("RESPAWN!"));

		GetWorld()->GetTimerManager().ClearTimer(RespawnCountdownTimerHandle);

		FTimerHandle HideHandle;
		GetWorld()->GetTimerManager().SetTimer(
			HideHandle,
			[ArenaModeHUD]()
			{
				ArenaModeHUD->HideCountdownWidget();
			},
			1.0f,
			false
		);
	}
}


