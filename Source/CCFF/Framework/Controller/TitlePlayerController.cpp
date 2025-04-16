#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/HUD/TitleHUD.h"
#include "Kismet/GameplayStatics.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(nullptr);
	SetInputMode(InputMode);
}

void ATitlePlayerController::JoinServer(const FString& InIPAddress)
{
	if (!InIPAddress.IsEmpty())
	{
		FName NextLevelName = FName(*InIPAddress);
		UGameplayStatics::OpenLevel(this, NextLevelName, true);
	}
}

void ATitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &ATitlePlayerController::HandleAnyKey);
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
