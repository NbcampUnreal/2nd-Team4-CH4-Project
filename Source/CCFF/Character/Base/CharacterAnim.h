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
	FCharacterAnim(): HittedMontage(nullptr) { AttackMontage.SetNum(8); }

	// Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation/Attack")
	TArray<UAnimMontage*> AttackMontage;

	// Hitted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation/Hitted")
	UAnimMontage* HittedMontage;
	
};
