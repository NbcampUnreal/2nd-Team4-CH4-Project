// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProfileWidget.generated.h"

class UImage;
class UProgressBar;
/**
 * 
 */
UCLASS()
class CCFF_API UProfileWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UProfileWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void UpdateHealthBar(const float InPercentage);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> HealthBar;
	
};
