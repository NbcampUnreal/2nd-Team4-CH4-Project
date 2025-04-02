// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseCharacter.h"
#include "BattleComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CCFF_API UBattleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBattleComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Counter")
	float CounterDamageModifier = 1.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Counter")
	float CounterHitStunModifier = 2.0f;

	FORCEINLINE void ResetCombo() { ComboCount = 0; };
	FORCEINLINE void IncreaseCombo() { ComboCount++; };

	float GetMeterGainFromDamageTaken(float Damage) const;
	FVector KnockbackDir(FVector KnockbackAngle, float KnockbackForce, FVector2D DiInput, float DiModifier) const;
	float ComboStaleDamage(float Damage, float MinimumDamage) const;
	int32 ComboStaleHitstun(int32 Hitstun) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	int32 ComboCount = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	float ComboDamageModifier = 0.8f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	float ComboHitStunModifier = 0.8f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	float ComboDiModifier = 1.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	float DiMultiplier = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	int32 MaxDiBonusThreshold = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	int32 MaxDiMultiplier = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Armor")
	int32 ArmorHitlag = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Armor")
	float ArmorDamageModifier = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meter")
	int32 MaxComboBonusThreshold = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meter")
	float MaxSuperMeterMultiplier = 4.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meter")
	float BurstGain = 5.0f;

	float ComboDiScaling() const;
};