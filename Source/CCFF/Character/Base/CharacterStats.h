// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStats.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterStats :public FTableRowBase
{
	GENERATED_BODY();
	
public:
	FCharacterStats():BurstMeterGain(0),BlockMeterGain(0),PerfectParryTick(0),AttackSuperGainMultiplier(0) { }
	

	// Battle property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/SystemBalance/Battle")
	float BurstMeterGain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/SystemBalance/Battle")
	float BlockMeterGain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/SystemBalance/Battle")
	float PerfectParryTick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/SystemBalance/Battle")
	float AttackSuperGainMultiplier;
	
};

USTRUCT(BlueprintType)
struct FCharacterBalanceStats :public FTableRowBase
{
	GENERATED_BODY();
public:
	FCharacterBalanceStats() : Gravity(0),GroundFriction(0),AirFriction(0),MaxAirSpeed(0),
								MaxWalkSpeed(0),MaxRunSpeed(0),MaxFallSpeed(0),DamageTakenModifier(0),DiModifier(0),DamageModifier(0) {}
	// Movement property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Movement")
	float Gravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Movement")
	float GroundFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Movement")
	float AirFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Movement")
	float MaxAirSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Movement")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Movement")
	float MaxRunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Movement")
	float MaxFallSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Battle")
	float DamageTakenModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Battle")
	float DiModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat/CharacterBalance/Battle")
	float DamageModifier;
};
