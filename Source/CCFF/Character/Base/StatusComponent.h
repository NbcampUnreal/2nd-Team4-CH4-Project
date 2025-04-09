// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHPChangedDelegate, float /*InCurrentHP*/);
DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHPChangedDelegate, float /*InMaxHP*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CCFF_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusComponent();

	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);
	
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UFUNCTION()
	void OnRep_Health();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/SystemBalance/Health")
	float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/SystemBalance/Health")
	float MaxHP;

	FOnCurrentHPChangedDelegate OnCurrentHPChanged;
	FOnDeathDelegate OnDeath;
	FOnMaxHPChangedDelegate OnMaxHPChanged;

};
