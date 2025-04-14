// Fill out your copyright notice in the Description page of Project Settings.


#include "HPWidgetComponent.h"
#include "Framework/UI/Character/UW_HPWidget.h"


// Sets default values for this component's properties
UHPWidgetComponent::UHPWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHPWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUW_HPWidget* HPText=Cast<UUW_HPWidget>(GetWidget());
	if (IsValid(HPText)==true)
	{
		HPText->SetOwningActor(GetOwner());
	}
}



