#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"

void ATrainingModeHUD::BeginPlay()
{
	Super::BeginPlay();

    APlayerController* PC = GetOwningPlayerController();
    if (PC && PC->IsLocalController() && TrainingWidgetClass)
    {
        TrainingWidget = CreateWidget<UTrainingWidget>(PC, TrainingWidgetClass);
        if (TrainingWidget)
        {
            TrainingWidget->AddToViewport();
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
