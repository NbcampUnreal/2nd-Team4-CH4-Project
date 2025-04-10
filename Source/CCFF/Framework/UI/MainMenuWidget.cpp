#include "Framework/UI/MainMenuWidget.h"
#include "Framework/UI/ConfirmPopupWidget.h"
#include "Framework/HUD/MainMenuHUD.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButtons = {
		PlayButton,
		LockerRoomButton,
		ShopButton,
		SettingButton,
		ExitGameButton
	};

	CurrentIndex = 0;

	if (PlayButton && !PlayButton->OnClicked.IsAlreadyBound(this, &UMainMenuWidget::HandlePlayClicked))
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandlePlayClicked);
	}

	if (SettingButton && !SettingButton->OnClicked.IsAlreadyBound(this, &UMainMenuWidget::HandleSettingClicked))
	{
		SettingButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleSettingClicked);
	}

	if (ExitGameButton && !ExitGameButton->OnClicked.IsAlreadyBound(this, &UMainMenuWidget::HandleExitGameClicked))
	{
		ExitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleExitGameClicked);
	}

	UpdateButtonFocus();
	SetKeyboardFocus();
}

FReply UMainMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Up)
	{
		CurrentIndex = (CurrentIndex - 1 + MenuButtons.Num()) % MenuButtons.Num();
		UpdateButtonFocus();
		return FReply::Handled();
	}
	else if (Key == EKeys::Down)
	{
		CurrentIndex = (CurrentIndex + 1 + MenuButtons.Num()) % MenuButtons.Num();
		UpdateButtonFocus();
		return FReply::Handled();
	}
	else if (Key == EKeys::Enter || Key == EKeys::SpaceBar)
	{
		HandleEnterPressed();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UMainMenuWidget::HandleEnterPressed()
{
	if (MenuButtons.IsValidIndex(CurrentIndex) && MenuButtons[CurrentIndex])
	{
		MenuButtons[CurrentIndex]->OnClicked.Broadcast();
	}
}

void UMainMenuWidget::HandlePlayClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		AMainMenuHUD* HUD = Cast<AMainMenuHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			HUD->ShowSelectModeWidget();
		}
	}
}

void UMainMenuWidget::HandleSettingClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		AMainMenuHUD* HUD = Cast<AMainMenuHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			HUD->ShowSettingsWidget();
		}
	}
}

void UMainMenuWidget::UpdateButtonFocus()
{
	for (int32 i = 0; i < MenuButtons.Num(); i++)
	{
		if (MenuButtons[i])
		{
			if (i == CurrentIndex)
			{
				MenuButtons[i]->SetKeyboardFocus();
			}
		}
	}
}

void UMainMenuWidget::HandleExitGameClicked()
{
	if (!ExitGamePopup && ExitGamePopupClass)
	{
		ExitGamePopup = CreateWidget<UConfirmPopupWidget>(GetWorld(), ExitGamePopupClass);
		if (ExitGamePopup)
		{
			ExitGamePopup->SetMessage(FText::FromString(TEXT("진짜 나감?")));
			ExitGamePopup->AddToViewport();

			if (!ExitGamePopup->OnConfirmPopupConfirmed.IsAlreadyBound(this, &UMainMenuWidget::HandleExitGameConfirmed))
			{
				ExitGamePopup->OnConfirmPopupConfirmed.AddDynamic(this, &UMainMenuWidget::HandleExitGameConfirmed);
			}

			if (!ExitGamePopup->OnConfirmPopupCanceled.IsAlreadyBound(this, &UMainMenuWidget::HandleExitGameCanceled))
			{
				ExitGamePopup->OnConfirmPopupCanceled.AddDynamic(this, &UMainMenuWidget::HandleExitGameCanceled);
			}
		}
	}
}

void UMainMenuWidget::HandleExitGameConfirmed()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
		}
	}
}

void UMainMenuWidget::HandleExitGameCanceled()
{
	if (ExitGamePopup)
	{
		ExitGamePopup->RemoveFromParent();
		ExitGamePopup = nullptr;
	}

	SetKeyboardFocus();
}
