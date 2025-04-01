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
	FCharacterStats():MaxHealth(0),Health(0){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
	float Health;
};
