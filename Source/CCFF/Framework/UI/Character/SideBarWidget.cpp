// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/Character/SideBarWidget.h"
#include "Components/ProgressBar.h"

USideBarWidget::USideBarWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void USideBarWidget::UpdateBurstMeterBar(const float InPercentage)
{
	BurstMeterBar->SetPercent(InPercentage);
}

void USideBarWidget::UpdateSuperMeterBar(const float InPercentage)
{
	SuperMeterBar->SetPercent(InPercentage);
}
