// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BattleModifiers.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBattleModifiers : public FTableRowBase
{
	GENERATED_BODY();
public:
	FBattleModifiers() : CounterDamageModifier(0), CounterHitlagModifier(0), CounterHitstunModifier(0), ComboCount(0), ComboDamageModifier(0), ComboHitstunModifier(0),
						 ComboDiModifier(0),DiMultiplier(0),MaxDiBonusThreshold(0),MaxDiMultiplier(0),ArmorHitlag(0),ArmorDamageModifier(0),
						 MaxComboBonusThreshold(0),MaxSuperMeterMultiplier(0),BurstGain(0) {}
	
	// Counter Modifiers
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Counter")
	float CounterDamageModifier;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Counter")
	float CounterHitlagModifier;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Counter")
	float CounterHitstunModifier;
	
	// Combo Modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 ComboCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float ComboDamageModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float ComboHitstunModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float ComboDiModifier;
	
	// DI Modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DI")
	float DiMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DI")
	int32 MaxDiBonusThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DI")
	int32 MaxDiMultiplier;
	
	// Armor Modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	int32 ArmorHitlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	float ArmorDamageModifier;
	
	// Meter Modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meter")
	int32 MaxComboBonusThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meter")
	float MaxSuperMeterMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meter")
	float BurstGain;
};
