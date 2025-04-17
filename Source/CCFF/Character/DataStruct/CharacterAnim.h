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
	FCharacterAnim(): HittedMontage(nullptr), GuardMontage(nullptr), DodgeMontage(nullptr), DeathMontage(nullptr) { AttackMontage.SetNum(16); }

	// Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> AttackMontage;

	// Hitted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> HittedMontage;

	//Guard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> GuardMontage;
	
	//Dodge
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;

	//Death
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;
};
