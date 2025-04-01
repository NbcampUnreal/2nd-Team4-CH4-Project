// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataLoaderSubSystem.generated.h"

/**
 * 
 */
struct FCharacterStats;

UCLASS()
class CCFF_API UDataLoaderSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDataLoaderSubSystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	FCharacterStats InitializeStat(const FName CharacterRowName) const;
private:
	//CharacterStats Data Table
	UPROPERTY()
	UDataTable* StatDataTable;
};

