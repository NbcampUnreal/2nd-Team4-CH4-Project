#include "Framework/HUD/BaseInGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/TogglePauseWidget.h"
#include "Framework/UI/SettingsWidget.h"

void ABaseInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	BaseInGameWidget = CreateAndAddWidget<UBaseInGameWidget>(BaseInGameWidgetClass, 2, ESlateVisibility::Visible);
	TogglePauseWidget = CreateAndAddWidget<UTogglePauseWidget>(TogglePauseWidgetClass, 3, ESlateVisibility::Collapsed);
	SettingsWidget = CreateAndAddWidget<USettingsWidget>(SettingsWidgetClass, 4, ESlateVisibility::Collapsed);
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

void ABaseInGameHUD::UpdateCharacterImageInWidget(FName CharacterID)
{
	if (BaseInGameWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("[ABaseInGameHUD] UpdateCharacterImageInWidget : Updated Profile Image for ID: %s"), *CharacterID.ToString());

		BaseInGameWidget->UpdateCharacterImage(CharacterID);
	}
}
