#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"

void ATrainingModeHUD::BeginPlay()
{
	Super::BeginPlay();

	TrainingWidget = CreateAndAddWidget<UTrainingWidget>(TrainingWidgetClass, 0, ESlateVisibility::Visible);
}

void ATrainingModeHUD::ShowTrainingWidget()
{
	if (IsValid(TrainingWidget))
	{
		TrainingWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
