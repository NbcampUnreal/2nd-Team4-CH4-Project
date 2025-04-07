#include "Framework/Controller/TitlePlayerController.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(nullptr);
	SetInputMode(InputMode);
}

void ATitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &ATitlePlayerController::HandleAnyKey);
		PushInputComponent(InputComponent);
	}
}

void ATitlePlayerController::HandleAnyKey(FKey PressedKey)
{
	ATitleHUD* HUD = Cast<ATitleHUD>(GetHUD());
	if (HUD)
	{
		HUD->NotifyAnyKeyPressed();
	}
}
