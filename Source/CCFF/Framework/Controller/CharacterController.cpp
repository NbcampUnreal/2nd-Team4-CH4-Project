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


ACharacterController::ACharacterController():
	DefaultMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
	GuardAction(nullptr),
	DodgeAction(nullptr),
	BurstAction(nullptr),
	bIsPause(false),
	PauseWidget(nullptr)
{
	AttackAction.SetNum(8);
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (UCCFFGameInstance* GI = Cast<UCCFFGameInstance>(GetGameInstance()))
		{
			const FString Nick = GI->GetNickname();
			UE_LOG(LogTemp, Log, TEXT("Client BeginPlay: Sending Nickname = '%s'"), *Nick);
			ServerSetNickname(Nick);
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

void ACharacterController::ServerSetNickname_Implementation(const FString& InNickname)
{
	if (AArenaPlayerState* PS = GetPlayerState<AArenaPlayerState>())
	{
		PS->SetPlayerNickname(InNickname);
		PS->SetPlayerName(InNickname);

		UE_LOG(LogTemp, Log, TEXT("ServerSetNickname: Received Nickname = '%s'"), *InNickname);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerSetNickname: PlayerState invalid"));
	}
}

bool ACharacterController::ServerSetNickname_Validate(const FString& InNickname)
{
	return true;
}

void ACharacterController::ServerSetCharacterID_Implementation(FName InID)
{
	if (AArenaPlayerState* ArenaPlayerState = GetPlayerState<AArenaPlayerState>())
	{
		ArenaPlayerState->SetSelectedCharacterID(InID);
		UE_LOG(LogTemp, Log, TEXT("[ServerSetCharacterID] SelectedCharacterID = %s"), *InID.ToString());
	}
}

bool ACharacterController::ServerSetCharacterID_Validate(FName InID)
{
	return true;
}

void ACharacterController::ClientSpectateCamera_Implementation(ACameraActor* SpectatorCam)
{
	if (!SpectatorCam)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClientSpectateCamera: SpectatorCam is null"));
		return;
	}

	UnPossess();
	ChangeState(NAME_Spectating);
	SetViewTargetWithBlend(SpectatorCam, 0.f);

	UE_LOG(LogTemp, Log, TEXT("ClientSpectateCamera: switched to SpectatorCamera"));
}