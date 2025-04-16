#include "Framework/HUD/TitleHUD.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/UI/LoginWidget.h"
#include "Kismet/GameplayStatics.h"

void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(TitleWidgetClass) == true)
	{
		TitleWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), TitleWidgetClass);
		if (IsValid(TitleWidgetInstance) == true)
		{
			TitleWidgetInstance->AddToViewport();
		}
	}

	SwitchUI(ETitleUIState::ETITLE_PRESSSTART);
}

void ATitleHUD::NotifyAnyKeyPressed()
{
	if (CurrentState == ETitleUIState::ETITLE_PRESSSTART)
	{
		SwitchUI(ETitleUIState::ETITLE_LOGIN);
		CurrentState = ETitleUIState::ETITLE_LOGIN;
	}
}

void ATitleHUD::SwitchUI(ETitleUIState UIState)
{
	RemoveCurrentWidget();

	TSubclassOf<UUserWidget> WidgetClass = nullptr;

	switch (UIState)
	{
	case ETitleUIState::ETITLE_PRESSSTART:
		WidgetClass = PressStartWidgetClass;
		break;
	case ETitleUIState::ETITLE_LOGIN:
		WidgetClass = LoginWidgetClass;
		break;
	default:
		break;
	}

	if (IsValid(WidgetClass) == true)
	{
		SubWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (IsValid(SubWidget) == true)
		{
			SubWidget->AddToViewport();

			if (ULoginWidget* LoginWidget = Cast<ULoginWidget>(SubWidget))
			{
				LoginWidget->OnLoginSuccess.AddDynamic(this, &ATitleHUD::HandleLoginSuccess);
			}
		}
	}

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetWidgetToFocus(SubWidget->TakeWidget());

		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

void ATitleHUD::RemoveCurrentWidget()
{
	if (IsValid(SubWidget) == true)
	{
		SubWidget->RemoveFromParent();
		SubWidget = nullptr;
	}
}

void ATitleHUD::HandleLoginSuccess()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainMenuMap"));
}
