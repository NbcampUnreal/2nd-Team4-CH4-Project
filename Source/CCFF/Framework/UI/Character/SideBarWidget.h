// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SideBarWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class CCFF_API USideBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USideBarWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void UpdateBurstMeterBar(const float InPercentage);
	
	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void UpdateSuperMeterBar(const float InPercentage);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> BurstMeterBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> SuperMeterBar;
};
