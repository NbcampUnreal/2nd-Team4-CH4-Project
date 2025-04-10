// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "LockerRoomWidget.generated.h"

DECLARE_DELEGATE(FOnBackToMainMenuRequested);

UCLASS()
class CCFF_API ULockerRoomWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Function to be called when the "Back" button is clicked
	UFUNCTION(BlueprintCallable)
	void RequestReturnToMainMenu();
	
	// Delegate to notify MainMenuHUD to return to the previous widget
	FOnBackToMainMenuRequested OnBackToMainMenuRequested;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

};
