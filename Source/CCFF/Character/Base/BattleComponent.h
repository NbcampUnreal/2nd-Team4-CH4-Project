// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Base/BattleModifiers.h"
#include "BattleComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CCFF_API UBattleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBattleComponent();

	FORCEINLINE void ResetCombo() { Modifiers.ComboCount = 0; };
	FORCEINLINE void IncreaseCombo() { Modifiers.ComboCount++; };

	float GetMeterGainFromDamageTaken(float Damage) const;
	FVector KnockbackDir(FVector KnockbackAngle, float KnockbackForce, FVector2D DiInput, float DiModifier) const;
	float ComboStaleDamage(float Damage, float MinimumDamage) const;
	int32 ComboStaleHitstun(int32 Hitstun) const;
	float ComboDiScaling() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifiers")
	FBattleModifiers Modifiers;
};