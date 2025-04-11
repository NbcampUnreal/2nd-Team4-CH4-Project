// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHPChangedDelegate, float /*InCurrentHP*/);
DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSuperMeterChangedDelegate, float /*InSuperMeter*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBurstMeterChangedDelegate, float /*InBurstMeter*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CCFF_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusComponent();
	
#pragma region GetFunction
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetBurstMeter() const { return BurstMeter; }
	FORCEINLINE float GetSuperMeter() const { return SuperMeter; }
	FORCEINLINE float GetMaxBurstMeter() const { return BurstMeter; }
	FORCEINLINE float GetMaxSuperMeter() const { return SuperMeter; }
#pragma endregion
	
#pragma region SetFunction
	void SetCurrentHP(float InCurrentHP);
	void SetMaxHP(float InMaxHP);
	void SetBurstMeter(float InBurstMeter);
	void SetSuperMeter(float InSuperMeter);
#pragma endregion

#pragma region AddFunction
	float AddCurrentHP(float Amount);
	float AddBurstMeter(float Amount);
	float AddSuperMeter(float Amount);
#pragma endregion
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UFUNCTION()
	void OnRep_CurrentHP();
	UFUNCTION()
	void OnRep_BurstMeter();
	UFUNCTION()
	void OnRep_SuperMeter();
public:
	FOnCurrentHPChangedDelegate OnCurrentHPChanged;
	FOnDeathDelegate OnDeath;
	FOnSuperMeterChangedDelegate OnSuperMeterChanged;
	FOnBurstMeterChangedDelegate OnBurstMeterChanged;
private:
	UPROPERTY()
	float MaxHP;
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHP)
	float CurrentHP;

	UPROPERTY()
	float MaxSuperMeter;
	UPROPERTY()
	float MaxBurstMeter;
	UPROPERTY(ReplicatedUsing=OnRep_BurstMeter)
	float BurstMeter;
	UPROPERTY(ReplicatedUsing=OnRep_SuperMeter)
	float SuperMeter;

};
