// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_HPWidget.h"
#include "Character/Base/BaseCharacter.h"
#include "StatusComponent.h"
#include "Components/TextBlock.h"

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
	OnMaxHPChange(InStatusComponent->GetMaxHP());
	OnCurrentHPChange(InStatusComponent->GetCurrentHP());
}

void UUW_HPWidget::OnMaxHPChange(float InMaxHP)
{
	int32 RoundedMaxHP=FMath::RoundToInt(InMaxHP);
	MaxHPText->SetText(FText::AsNumber(RoundedMaxHP));
}

void UUW_HPWidget::OnCurrentHPChange(float InCurrentHP)
{
	int32 RoundedCurrentHP=FMath::RoundToInt(InCurrentHP);
	CurrentHPText->SetText(FText::AsNumber(RoundedCurrentHP));
}
