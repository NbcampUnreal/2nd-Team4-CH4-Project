#include "Framework/UI/SettingsWidget.h"
#include "Framework/UI/AudioSettingsWidget.h"
#include "Framework/UI/GraphicSettingsWidget.h"
#include "Framework/UI/ControlSettingsWidget.h"
#include "Framework/UI/Settings/ApplySettingsInterface.h"
#include "Framework/HUD/MainMenuHUD.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Components/Button.h"

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize tab button list for focus navigation
	TabButtons = {
		AudioTabButton,
		GraphicTabButton,
		ControlTabButton
	};
	CurrentTabIndex = 0;
	UpdateTabFocus();

	// Bind tab button click events
	if (AudioTabButton)
	{
		AudioTabButton->OnClicked.AddDynamic(this, &USettingsWidget::OnAudioTabClicked);
	}
	
	if (GraphicTabButton)
	{
		GraphicTabButton->OnClicked.AddDynamic(this, &USettingsWidget::OnGraphicTabClicked);
	}
	
	if (ControlTabButton)
	{
		ControlTabButton->OnClicked.AddDynamic(this, &USettingsWidget::OnControlTabClicked);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &USettingsWidget::OnApplyClicked);
	}
	if (ResetButton)
	{
		ResetButton->OnClicked.AddDynamic(this, &USettingsWidget::OnResetClicked);
	}
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &USettingsWidget::OnCancelClicked);
	}

	// Display the audio settings tab by default
	OnAudioTabClicked();
}

FReply USettingsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// Handle Tab key for switching between tab buttons
	if (InKeyEvent.GetKey() == EKeys::Tab)
	{
		CurrentTabIndex = (CurrentTabIndex + 1) % TabButtons.Num();
		UpdateTabFocus();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void USettingsWidget::UpdateTabFocus()
{
	// Set keyboard focus to the current tab button
	for (int32 i = 0; i < TabButtons.Num(); i++)
	{
		if (TabButtons[i])
		{
			if (i == CurrentTabIndex)
			{
				TabButtons[i]->SetKeyboardFocus();
			}
		}
	}
}

void USettingsWidget::OnAudioTabClicked()
{
	SwitchSettingsWidget<UAudioSettingsWidget>(AudioSettingsWidgetClass);
}

void USettingsWidget::OnGraphicTabClicked()
{
	SwitchSettingsWidget<UGraphicSettingsWidget>(GraphicSettingsWidgetClass);
}

void USettingsWidget::OnControlTabClicked()
{
	SwitchSettingsWidget<UControlSettingsWidget>(ControlSettingsWidgetClass);
}

void USettingsWidget::OnApplyClicked()
{
	if (CurrentSettingsWidget && CurrentSettingsWidget->Implements<UApplySettingsInterface>())
	{
		IApplySettingsInterface::Execute_ApplySettings(CurrentSettingsWidget);
	}
}

void USettingsWidget::OnResetClicked()
{
	if (CurrentSettingsWidget && CurrentSettingsWidget->Implements<UApplySettingsInterface>())
	{
		IApplySettingsInterface::Execute_ResetSettings(CurrentSettingsWidget);
	}
}

void USettingsWidget::OnCancelClicked()
{
	if (CurrentSettingsWidget && CurrentSettingsWidget->Implements<UApplySettingsInterface>())
	{
		IApplySettingsInterface::Execute_CancelSettings(CurrentSettingsWidget);
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		AHUD* CurrentHUD = PlayerController->GetHUD();
		if (CurrentHUD)
		{
			if (AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(CurrentHUD))
			{
				MainMenuHUD->HideSettingsWidget();
			}
			else if (ABaseInGameHUD* BaseInGameHUD = Cast<ABaseInGameHUD>(CurrentHUD))
			{
				BaseInGameHUD->HideSettingsWidget();
			}
		}
	}
}
