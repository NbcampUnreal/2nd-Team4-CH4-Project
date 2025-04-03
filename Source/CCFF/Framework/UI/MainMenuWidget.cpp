#include "Framework/UI/MainMenuWidget.h"
#include "Framework/UI/QuitPopupWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButtons = {
		GameSessionButton,
		LockerRoomButton,
		ShopButton,
		SettingButton,
		QuitButton
	};

	CurrentIndex = 0;

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleQuitClicked);
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

void UMainMenuWidget::HandleQuitClicked()
{
	if (!QuitPopup && QuitPopupClass)
	{
		QuitPopup = CreateWidget<UQuitPopupWidget>(GetWorld(), QuitPopupClass);
		if (QuitPopup)
		{
			QuitPopup->AddToViewport();
			QuitPopup->OnQuitConfirmed.AddDynamic(this, &UMainMenuWidget::HandleQuitConfirmed);
			QuitPopup->OnQuitCanceled.AddDynamic(this, &UMainMenuWidget::HandleQuitCanceled);
		}
	}
}

void UMainMenuWidget::HandleQuitConfirmed()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
		}
	}
}

void UMainMenuWidget::HandleQuitCanceled()
{
	if (QuitPopup)
	{
		QuitPopup->RemoveFromParent();
		QuitPopup = nullptr;
	}

	SetKeyboardFocus();
}
