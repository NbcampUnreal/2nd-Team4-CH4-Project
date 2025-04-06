#include "Framework/HUD/BaseInGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/TogglePauseWidget.h"
#include "Framework/UI/SettingsWidget.h"

void ABaseInGameHUD::BeginPlay()
{
	BaseInGameWidget = CreateAndAddWidget<UBaseInGameWidget>(BaseInGameWidgetClass, 0, ESlateVisibility::Visible);
	TogglePauseWidget = CreateAndAddWidget<UTogglePauseWidget>(TogglePauseWidgetClass, 1, ESlateVisibility::Collapsed);
	SettingsWidget = CreateAndAddWidget<USettingsWidget>(SettingsWidgetClass, 2, ESlateVisibility::Collapsed);
}

void ABaseInGameHUD::ShowTogglePauseWidget()
{
	if (IsValid(TogglePauseWidget))
	{
		TogglePauseWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseInGameHUD::ShowSettingsWidget()
{
	if (IsValid(SettingsWidget))
	{
		SettingsWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseInGameHUD::HideSettingsWidget()
{
	if (IsValid(SettingsWidget))
	{
		SettingsWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

