#include "Framework/HUD/CCFFOutGameHUD.h"
#include "Blueprint/UserWidget.h"

void ACCFFOutGameHUD::BeginPlay()
{
	Super::BeginPlay();
	SwitchUI(EOutGameUIState::EOGUI_TITLE);
}

void ACCFFOutGameHUD::SwitchUI(EOutGameUIState UIState)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	TSubclassOf<UUserWidget> WidgetClassToUse = nullptr;

	switch (UIState)
	{
	case EOutGameUIState::EOGUI_TITLE:
		WidgetClassToUse = TitleWidgetClass;
		break;
	case EOutGameUIState::EOGUI_LOGIN:
		WidgetClassToUse = LoginWidgetClass;
		break;
	case EOutGameUIState::EOGUI_MAINMENU:
		WidgetClassToUse = MainMenuWidgetClass;
		break;
	default:
		break;
	}

	if (WidgetClassToUse)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClassToUse);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
