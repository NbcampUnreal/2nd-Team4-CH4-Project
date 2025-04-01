#include "Framework/Controller/CCFFOutgamePlayerController.h"

void ACCFFOutgamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("AnyKey", IE_Pressed, this, &ACCFFOutgamePlayerController::HandleAnyKey);
	}
}

void ACCFFOutgamePlayerController::HandleAnyKey()
{
	RequestUIChange(EOutGameUIState::EOGUI_LOGIN);
}

void ACCFFOutgamePlayerController::RequestUIChange(EOutGameUIState NextState)
{
	if (auto* HUD = Cast<ACCFFOutGameHUD>(GetHUD()))
	{
		HUD->SwitchUI(NextState);
	}
}
