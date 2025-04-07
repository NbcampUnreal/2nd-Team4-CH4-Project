#include "Framework/HUD/TitleHUD.h"
#include "Framework/UI/LoginWidget.h"
#include "Kismet/GameplayStatics.h"

void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();

	if (TitleWidgetClass)
	{
		TitleWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), TitleWidgetClass);
		if (TitleWidgetInstance)
		{
			TitleWidgetInstance->AddToViewport();
		}
	}

	SwitchUI(EOutGameUIState::EOGUI_TITLE);
}

void ATitleHUD::SwitchUI(EOutGameUIState UIState)
{
	if (SubWidget)
	{
		SubWidget->RemoveFromParent();
		SubWidget = nullptr;
	}

	TSubclassOf<UUserWidget> WidgetClass = nullptr;

	switch (UIState)
	{
	case EOutGameUIState::EOGUI_TITLE:
		WidgetClass = PressStartWidgetClass;
		break;
	case EOutGameUIState::EOGUI_LOGIN:
		WidgetClass = LoginWidgetClass;
		break;
	default:
		break;
	}

	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[HUD] WidgetClass is NULL for UIState %d"), static_cast<int32>(UIState));
		return;
	}

	SubWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);


	if (WidgetClass)
	{
		SubWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (SubWidget)
		{
			SubWidget->AddToViewport();

			if (ULoginWidget* Login = Cast<ULoginWidget>(SubWidget))
			{
				Login->OnLoginSuccess.AddDynamic(this, &ATitleHUD::HandleLoginSuccess);
			}
		}
	}
}

void ATitleHUD::NotifyAnyKeyPressed()
{
	SwitchUI(EOutGameUIState::EOGUI_LOGIN);
}

void ATitleHUD::HandleLoginSuccess()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainMenuMap"));
}
