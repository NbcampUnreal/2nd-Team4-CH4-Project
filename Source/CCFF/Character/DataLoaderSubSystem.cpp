// Fill out your copyright notice in the Description page of Project Settings.


#include "DataLoaderSubSystem.h"
#include "Base/CharacterStats.h"
#include "Base/BattleModifiers.h"
#include "Base/CharacterAnim.h"
#include "Base/AttackCollisionData.h"

UDataLoaderSubSystem::UDataLoaderSubSystem()
{
	// Find Character Stats Data Table and Initialize Data Table Variable
	static ConstructorHelpers::FObjectFinder<UDataTable> StatDataFinder(TEXT("/Game/CCFF/DataTables/DT_Stats.DT_Stats"));
	if (StatDataFinder.Succeeded())
	{
		StatDataTable=StatDataFinder.Object;
	}
	// Find Character Stats Data Table and Initialize Data Table Variable
	static ConstructorHelpers::FObjectFinder<UDataTable> BalanceStatDataFinder(TEXT("/Game/CCFF/DataTables/DT_BalanceStats.DT_BalanceStats"));
	if (BalanceStatDataFinder.Succeeded())
	{
		BalanceStatsDataTable=BalanceStatDataFinder.Object;
	}

	// Find Character Stats Data Table and Initialize Data Table Variable
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAnimDataFinder(TEXT("/Game/CCFF/DataTables/DT_CharacterAnimation.DT_CharacterAnimation"));
	if (CharacterAnimDataFinder.Succeeded())
	{
		CharacterAnimDataTable=CharacterAnimDataFinder.Object;
	}

	// Find Character Stats Data Table and Initialize Data Table Variable
	static ConstructorHelpers::FObjectFinder<UDataTable> AttackCollisionDataFinder(TEXT("/Game/CCFF/DataTables/DT_AttackData.DT_AttackData"));
	if (AttackCollisionDataFinder.Succeeded())
	{
		AttackCollisionDataTable=AttackCollisionDataFinder.Object;
	}
	
	// Find Character Stats Data Table and Initialize Data Table Variable
	static ConstructorHelpers::FObjectFinder<UDataTable> HitDataFinder(TEXT("/Game/CCFF/DataTables/DT_HitData.DT_HitData"));
	if (HitDataFinder.Succeeded())
	{
		HitBoxDataTable=HitDataFinder.Object;
	}
	
	// Find BattleModifiers Data Table and Initialize Data Table Variable
	static ConstructorHelpers::FObjectFinder<UDataTable> BattleModifiersDataFinder(TEXT("/Game/CCFF/DataTables/DT_BattleModifiers.DT_BattleModifiers"));
	if (BattleModifiersDataFinder.Succeeded())
	{
		BattleModifiersDataTable=BattleModifiersDataFinder.Object;
	}
}

void UDataLoaderSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

FCharacterStats UDataLoaderSubSystem::InitializeStat(const FName RowName) const
{
	if (RowName.IsValid()&&StatDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		if (FCharacterStats* TempStats=StatDataTable->FindRow<FCharacterStats>(RowName,TEXT("")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Success!"));
			return *TempStats;
		}
	}
	return FCharacterStats();
}

FCharacterBalanceStats UDataLoaderSubSystem::InitializeBalanceStat(const FName RowName) const
{
	if (RowName.IsValid()&&BalanceStatsDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		if (FCharacterBalanceStats* TempStats=BalanceStatsDataTable->FindRow<FCharacterBalanceStats>(RowName,TEXT("")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Success!"));
			return *TempStats;
		}
	}
	return FCharacterBalanceStats();
}

FBattleModifiers UDataLoaderSubSystem::InitializeBattleModifiers(const FName RowName) const
{
	if (RowName.IsValid()&&BattleModifiersDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		if (FBattleModifiers* TempStats=BattleModifiersDataTable->FindRow<FBattleModifiers>(RowName,TEXT("")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Success!"));
			return *TempStats;
		}
	}
	return FBattleModifiers();
}

FAttackCollisionData UDataLoaderSubSystem::InitializeAttackCollisionData(const FName RowName) const
{
	if (RowName.IsValid()&&AttackCollisionDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		if (FAttackCollisionData* TempStats=AttackCollisionDataTable->FindRow<FAttackCollisionData>(RowName,TEXT("")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Success!"));
			return *TempStats;
		}
	}
	return FAttackCollisionData();
}

FHitBoxData UDataLoaderSubSystem::InitializeHitBoxData(const FName RowName) const
{
	if (RowName.IsValid()&&HitBoxDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		if (FHitBoxData* TempStats=HitBoxDataTable->FindRow<FHitBoxData>(RowName,TEXT("")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Success!"));
			return *TempStats;
		}
	}
	return FHitBoxData();
}

FCharacterAnim UDataLoaderSubSystem::InitializeCharacterAnim(const FName RowName) const
{
	if (RowName.IsValid()&&CharacterAnimDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
		if (FCharacterAnim* TempStats=CharacterAnimDataTable->FindRow<FCharacterAnim>(RowName,TEXT("")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Success!"));
			return *TempStats;
		}
	}
	return FCharacterAnim();
}
