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
	FCharacterStats(): Gravity(0),GroundFriction(0),AirFriction(0),MaxAirSpeed(0),MaxWalkSpeed(0),MaxRunSpeed(0),MaxFallSpeed(0),
					   MaxSuperMeter(0),SuperMeter(0),MaxBurstMeter(0),BurstMeter(0),BurstMeterGain(0),MaxBlockMeter(0),BlockMeter(0),
					   BlockMeterGain(0),PerfectParryTick(0),DamageTakenModifier(0),DiModifier(0),MaxHealth(0),Health(0) {}
	
	// Movement property
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Movement")
	float Gravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Movement")
	float GroundFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Movement")
	float AirFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Movement")
	float MaxAirSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Movement")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Movement")
	float MaxRunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Movement")
	float MaxFallSpeed;

	// Battle property
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float MaxSuperMeter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float SuperMeter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float MaxBurstMeter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float BurstMeter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float BurstMeterGain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float MaxBlockMeter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float BlockMeter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float BlockMeterGain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float PerfectParryTick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float DamageTakenModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Battle")
	float DiModifier;
	
	// Health property
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat/Health")
	float Health;
};
