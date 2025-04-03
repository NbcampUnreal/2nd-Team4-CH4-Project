#include "Framework/UI/MainMenuWidget.h"
#include "Framework/UI/ConfirmPopupWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButtons = {
		EnterArenaButton,
		LockerRoomButton,
		ShopButton,
		SettingButton,
		ExitGameButton
	};

	CurrentIndex = 0;

	if (ExitGameButton)
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
			ExitGamePopup->OnConfirmPopupConfirmed.AddDynamic(this, &UMainMenuWidget::HandleExitGameConfirmed);
			ExitGamePopup->OnConfirmPopupCanceled.AddDynamic(this, &UMainMenuWidget::HandleExitGameCanceled);
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
