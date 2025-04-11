// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_HPWidget.h"
#include "Character/Base/BaseCharacter.h"
#include "StatusComponent.h"
#include "Components/ProgressBar.h"

UUW_HPWidget::UUW_HPWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_HPWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ABaseCharacter* OwningCharacter=Cast<ABaseCharacter>(OwningActor);
	if (IsValid(OwningCharacter)==true)
	{
		OwningCharacter->SetHPWidget(this);
	}
}

void UUW_HPWidget::InitializeHPWidget(UStatusComponent* InStatusComponent)
{
	const float Percentage=FMath::Clamp(InStatusComponent->GetCurrentHP()/InStatusComponent->GetMaxHP(), 0, InStatusComponent->GetMaxHP());
	OnHPChange(Percentage);
}

void UUW_HPWidget::OnHPChange(const float InPercentage)
{
	HPBar->SetPercent(InPercentage);
}
