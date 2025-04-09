// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_HPWidget.generated.h"

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
	void OnMaxHPChange(float InMaxHP);

	UFUNCTION()
	void OnCurrentHPChange(float InCurrentHP);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentHPText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHPText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;
};
