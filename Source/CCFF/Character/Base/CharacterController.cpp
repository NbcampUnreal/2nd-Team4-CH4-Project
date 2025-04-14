#include "CharacterController.h"
#include "Blueprint/UserWidget.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/UI/TogglePauseWidget.h"
#include "Framework/GameMode/TrainingGameMode.h"
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "Framework/GameMode/ArenaGameMode.h"
#include "Engine/World.h"


ACharacterController::ACharacterController()
   : DefaultMappingContext(nullptr),
     MoveAction(nullptr),
     JumpAction(nullptr),
	 AttackAction1(nullptr),
	 AttackAction2(nullptr),
	 AttackAction3(nullptr),
	 AttackAction4(nullptr),
	 PauseWidget(nullptr),
	 bIsPause(false)
{
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();


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
