// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/ProfileWidget.h"
#include "Components/ProgressBar.h"

UProfileWidget::UProfileWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UProfileWidget::UpdateHealthBar(const float InPercentage)
{
	HealthBar->SetPercent(InPercentage);
}
