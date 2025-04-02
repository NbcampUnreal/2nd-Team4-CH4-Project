// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataLoaderSubSystem.generated.h"

/**
 * 
 */
struct FCharacterStats;
struct FCharacterAnim;
struct FAttackCollisionData;
struct FBattleModifiers;

UCLASS()
class CCFF_API UDataLoaderSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDataLoaderSubSystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	FCharacterStats InitializeStat(const FName RowName) const;
	
	UFUNCTION(BlueprintCallable)
	FBattleModifiers InitializeBattleModifiers(const FName RowName) const;

	UFUNCTION(BlueprintCallable)
	FAttackCollisionData InitializeAttackCollisionData(const FName RowName) const;

	UFUNCTION(BlueprintCallable)
	FCharacterAnim InitializeCharacterAnim(const FName RowName) const;
	
private:
	//CharacterStats Data Table
	UPROPERTY()
	UDataTable* StatDataTable;

	//CollisionData Data Table
	UPROPERTY()
	UDataTable* AttackCollisionDataTable;

	//CharacterAnim Data Table
	UPROPERTY()
	UDataTable* CharacterAnimDataTable;
	
	//BattleModifiers Data Table
	UPROPERTY()
	UDataTable* BattleModifiersDataTable;
};

