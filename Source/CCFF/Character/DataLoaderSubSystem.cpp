// Fill out your copyright notice in the Description page of Project Settings.


#include "DataLoaderSubSystem.h"
#include "Base/CharacterStats.h"

UDataLoaderSubSystem::UDataLoaderSubSystem()
{
	// Find Character Stats Data Table and Initialize Data Table Variable
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/Character/DataTables/DT_Stats.DT_Stats"));
	if (DataTableFinder.Succeeded())
	{
		StatDataTable=DataTableFinder.Object;
	}
}

void UDataLoaderSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

FCharacterStats UDataLoaderSubSystem::InitializeStat(const FName CharacterRowName) const
{
	if (CharacterRowName.IsValid()&&StatDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterRowName: %s"), *CharacterRowName.ToString());
		if (FCharacterStats* TempStats=StatDataTable->FindRow<FCharacterStats>(CharacterRowName,TEXT("")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Success!"));
			return *TempStats;
		}
	}
	return FCharacterStats();
}
