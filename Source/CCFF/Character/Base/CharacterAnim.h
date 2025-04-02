// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterAnim.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterAnim :public FTableRowBase
{
	GENERATED_BODY();
public:
	FCharacterAnim(): AttackMontage1(nullptr),AttackMontage2(nullptr),HittedMontage(nullptr) {}

	// Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation/Attack")
	UAnimMontage* AttackMontage1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation/Attack")
	UAnimMontage* AttackMontage2;

	// Hitted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation/Hitted")
	UAnimMontage* HittedMontage;
	
};
