#include "Framework/UI/SelectModeWidget.h"
#include "Framework/UI/ConfirmPopupWidget.h"
#include "Framework/HUD/MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void USelectModeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AdventureButton)
	{
		AdventureButton->OnClicked.AddDynamic(this, &USelectModeWidget::OnAdventrueClicked);
	}
	
	if (ArenaButton)
	{
		ArenaButton->OnClicked.AddDynamic(this, &USelectModeWidget::OnArenaClicked);
	}

	if (TrainingRoomButton)
	{
		TrainingRoomButton->OnClicked.AddDynamic(this, &USelectModeWidget::OnTrainingRoomClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USelectModeWidget::OnBackClicked);
	}
}

void USelectModeWidget::OnAdventrueClicked()
{
	// Todo: 
}

void USelectModeWidget::OnArenaClicked()
{
	// Todo: 
}

void USelectModeWidget::OnTrainingRoomClicked()
{
	if (!TrainingRoomPopup && ConfirmPopupClass)
	{
		TrainingRoomPopup = CreateWidget<UConfirmPopupWidget>(GetWorld(), ConfirmPopupClass);
		TrainingRoomPopup->SetMessage(FText::FromString(TEXT("Time to warm up?")));
		TrainingRoomPopup->AddToViewport();
		TrainingRoomPopup->OnConfirmPopupConfirmed.AddDynamic(this, &USelectModeWidget::EnterTrainingRoom);
		TrainingRoomPopup->OnConfirmPopupCanceled.AddDynamic(this, &USelectModeWidget::CancelPopup);
	}
}

void USelectModeWidget::EnterTrainingRoom()
{
	UGameplayStatics::OpenLevel(this, FName("L_TrainingField"));
}

void USelectModeWidget::CancelPopup()
{
	TrainingRoomPopup = nullptr;
	SetKeyboardFocus();
}

void USelectModeWidget::OnBackClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		AMainMenuHUD* HUD = Cast<AMainMenuHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			HUD->ShowMainMenuWidget();
		}
	}
}
