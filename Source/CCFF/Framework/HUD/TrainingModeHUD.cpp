#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"

void ATrainingModeHUD::BeginPlay()
{
	Super::BeginPlay();

    APlayerController* MyPlayerController = GetOwningPlayerController();
    if (MyPlayerController && MyPlayerController->IsLocalController() && TrainingWidgetClass)
    {
        TrainingWidget = CreateWidget<UTrainingWidget>(MyPlayerController, TrainingWidgetClass);
        if (IsValid(TrainingWidget))
        {
            TrainingWidget->AddToViewport(4);
        }
    }
}

void ATrainingModeHUD::ShowTrainingWidget()
{
	if (IsValid(TrainingWidget))
	{
		TrainingWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
