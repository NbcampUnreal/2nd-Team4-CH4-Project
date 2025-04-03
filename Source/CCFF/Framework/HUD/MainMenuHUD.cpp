#include "Framework/HUD/MainMenuHUD.h"
#include "Framework/UI/MainMenuWidget.h"
#include "Framework/UI/SelectModeWidget.h"
#include "Framework/UI/SettingsWidget.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenuWidget();
}

void AMainMenuHUD::ShowMainMenuWidget()
{
	SwitchWidget(MainMenuWidget, MainMenuWidgetClass);
}

void AMainMenuHUD::ShowSelectModeWidget()
{
	SwitchWidget(SelectModeWidget, SelectModeWidgetClass);
}

void AMainMenuHUD::ShowSettingsWidget()
{
	SwitchWidget(SettingsWidget, SettingsWidgetClass, 10);
}

void AMainMenuHUD::HideSettingsWidget()
{
	if (SettingsWidget)
	{
		SettingsWidget->RemoveFromParent();
		SettingsWidget = nullptr;
	}
}
