#include "Framework/UI/SelectModeWidget.h"
#include "Framework/UI/ConfirmPopupWidget.h"
#include "Framework/UI/ArenaModeWidget.h"
#include "Framework/HUD/MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void USelectModeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AdventureButton && !AdventureButton->OnClicked.IsAlreadyBound(this, &USelectModeWidget::OnAdventureClicked))
	{
		AdventureButton->OnClicked.AddDynamic(this, &USelectModeWidget::OnAdventureClicked);
	}
	
	if (ArenaButton && !ArenaButton->OnClicked.IsAlreadyBound(this, &USelectModeWidget::OnArenaClicked))
	{
		ArenaButton->OnClicked.AddDynamic(this, &USelectModeWidget::OnArenaClicked);
	}

	if (TrainingRoomButton && !TrainingRoomButton->OnClicked.IsAlreadyBound(this, &USelectModeWidget::OnTrainingRoomClicked))
	{
		TrainingRoomButton->OnClicked.AddDynamic(this, &USelectModeWidget::OnTrainingRoomClicked);
	}

	if (BackButton && !BackButton->OnClicked.IsAlreadyBound(this, &USelectModeWidget::OnBackClicked))
	{
		BackButton->OnClicked.AddDynamic(this, &USelectModeWidget::OnBackClicked);
	}
}

void USelectModeWidget::OnAdventureClicked()
{
	// Todo: 
}

void USelectModeWidget::OnArenaClicked()
{
	OnArenaModeRequested.Broadcast();
}

void USelectModeWidget::OnTrainingRoomClicked()
{
	if (!TrainingRoomPopup && ConfirmPopupClass)
	{
		TrainingRoomPopup = CreateWidget<UConfirmPopupWidget>(GetWorld(), ConfirmPopupClass);
		TrainingRoomPopup->SetMessage(FText::FromString(TEXT("Time to warm up?")));
		TrainingRoomPopup->AddToViewport();
		TrainingRoomPopup->OnConfirmPopupConfirmed.AddDynamic(this, &USelectModeWidget::HandleTrainingPopupConfirmed);
		TrainingRoomPopup->OnConfirmPopupCanceled.AddDynamic(this, &USelectModeWidget::HandleTrainingPopupCanceled);
	}
}

void USelectModeWidget::HandleTrainingPopupConfirmed()
{
	UGameplayStatics::OpenLevel(this, FName("TestTrainingMap"));
}

void USelectModeWidget::HandleTrainingPopupCanceled()
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
