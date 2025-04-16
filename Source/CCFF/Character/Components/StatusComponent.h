// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHPChangedDelegate, float /*InPercentage*/);
DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSuperMeterChangedDelegate, float /*InPercentage*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBurstMeterChangedDelegate, float /*InPercentage*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStockCountChangedDelegate, int32 /*InCount*/);
DECLARE_MULTICAST_DELEGATE(FOnBlockMeterChangedDelegate);

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
	FORCEINLINE float GetMaxBurstMeter() const { return BurstMeter; }
	FORCEINLINE float GetSuperMeter() const { return SuperMeter; }
	FORCEINLINE float GetMaxSuperMeter() const { return SuperMeter; }
	FORCEINLINE float GetMaxBlockMeter() const { return MaxBlockMeter; }
	FORCEINLINE float GetBlockMeter() const { return BlockMeter; }
#pragma endregion
	
#pragma region SetFunction
	void SetCurrentHP(const float InCurrentHP);
	void SetSuperMeter(const float InSuperMeter);
	void SetBurstMeter(const float InBurstMeter);
	void SetBlockMeter(const float InBlockMeter);
#pragma endregion
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UFUNCTION()
	void OnRep_CurrentHP();
	UFUNCTION()
	void OnRep_BurstMeter();
	UFUNCTION()
	void OnRep_SuperMeter();
	UFUNCTION()
	void OnRep_StockCount();
public:
	FOnCurrentHPChangedDelegate OnCurrentHPChanged;
	FOnDeathDelegate OnDeathState;
	FOnSuperMeterChangedDelegate OnSuperMeterChanged;
	FOnBurstMeterChangedDelegate OnBurstMeterChanged;
	FOnBlockMeterChangedDelegate OnGuardCrush;
	FOnStockCountChangedDelegate OnStockCountChanged;
private:
	UPROPERTY()
	float MaxHP;
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHP)
	float CurrentHP;

	UPROPERTY()
	float MaxSuperMeter;
	UPROPERTY(ReplicatedUsing=OnRep_SuperMeter)
	float SuperMeter;
	UPROPERTY()
	float MaxBurstMeter;
	UPROPERTY(ReplicatedUsing=OnRep_BurstMeter)
	float BurstMeter;
	UPROPERTY()
	float MaxBlockMeter;
	UPROPERTY(Replicated)
	float BlockMeter;

	UPROPERTY(ReplicatedUsing=OnRep_StockCount)
	int32 CurrentStockCount;

};
