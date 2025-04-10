// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/LockerRoomWidget.h"
#include "Components/Button.h"

void ULockerRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton && !BackButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::RequestReturnToMainMenu))
	{
		BackButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::RequestReturnToMainMenu);
	}
}

void ULockerRoomWidget::RequestReturnToMainMenu()
{
	if (OnBackToMainMenuRequested.IsBound())
	{
		OnBackToMainMenuRequested.Execute();
	}
}
