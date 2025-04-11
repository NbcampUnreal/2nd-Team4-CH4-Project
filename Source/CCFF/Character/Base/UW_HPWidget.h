// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_HPWidget.generated.h"

class UProgressBar;
class UStatusComponent;
class UTextBlock;
/**
 * 
 */
UCLASS()
class CCFF_API UUW_HPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_HPWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void InitializeHPWidget(UStatusComponent* InStatusComponent);

	FORCEINLINE AActor* GetOwningActor() const { return OwningActor; }

	FORCEINLINE void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

	UFUNCTION()
	void OnHPChange(const float InPercentage);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> HPBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;
};
