#include "Framework/HUD/TitleHUD.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/UI/LoginWidget.h"
#include "Kismet/GameplayStatics.h"

ATitleHUD::ATitleHUD()
	: TitleWidgetInstance(nullptr), SubWidget(nullptr)
{
}

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
	if (IsValid(SubWidget) == true)
	{
		SwitchUI(ETitleUIState::ETITLE_LOGIN);
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
