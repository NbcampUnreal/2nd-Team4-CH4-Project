// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/Character/ProfileWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UProfileWidget::UProfileWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UProfileWidget::UpdateHealthBar(const float InPercentage)
{
	HealthBar->SetPercent(InPercentage);
}

void UProfileWidget::UpdateStockCount(const int32 InCount)
{
	FText CountText=FText::FromString(FString::FormatAsNumber(InCount));
	StockCount->SetText(CountText);
	UE_LOG(LogTemp, Display, TEXT("UProfileWidget::UpdateStockCount: %s"),*CountText.ToString());
}
