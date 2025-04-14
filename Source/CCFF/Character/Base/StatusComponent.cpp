// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	//Initialize Status
	//MaxHP=10000;
	MaxHP=1000;
	CurrentHP=MaxHP;
	SuperMeter=0;
	MaxSuperMeter=10000;
	BurstMeter=0;
	MaxBurstMeter=10000;
	MaxBlockMeter=10000;
	BlockMeter=10000;
	//Set Replicate
	SetIsReplicatedByDefault(true);
}

void UStatusComponent::SetCurrentHP(float InCurrentHP)
{
	CurrentHP = InCurrentHP;
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnDeathState.Broadcast();
	}
	float Percentage = FMath::Clamp(CurrentHP / MaxHP,0.f,1.f);
	OnCurrentHPChanged.Broadcast(Percentage);
}

void UStatusComponent::SetBurstMeter(float InBurstMeter)
{
	BurstMeter = InBurstMeter;
	if (BurstMeter <= KINDA_SMALL_NUMBER)
	{
		BurstMeter = 0.f;
	}
	float Percentage = FMath::Clamp(BurstMeter / MaxBurstMeter,0.f,1.f);
	OnBurstMeterChanged.Broadcast(Percentage);
}

void UStatusComponent::SetSuperMeter(float InSuperMeter)
{
	SuperMeter = InSuperMeter;
	if (SuperMeter <= KINDA_SMALL_NUMBER)
	{
		SuperMeter = 0.f;
	}
	float Percentage = FMath::Clamp(SuperMeter / MaxSuperMeter,0.f,1.f);
	OnSuperMeterChanged.Broadcast(Percentage);
}

void UStatusComponent::SetBlockMeter(const float InBlockMeter)
{
	BlockMeter=InBlockMeter;
	if (BlockMeter <= 0.0f)
	{
		OnGuardCrush.Broadcast();
	}
}

void UStatusComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass,CurrentHP);
	DOREPLIFETIME_CONDITION(ThisClass,SuperMeter,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass,BurstMeter,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass,BlockMeter,COND_OwnerOnly);
}

void UStatusComponent::OnRep_CurrentHP()
{
	float Percentage = FMath::Clamp(CurrentHP / MaxHP,0.f,1.f);
	//UE_LOG(LogTemp,Log,TEXT("OnRep_CurrentHP: %0.1f"),Percentage);
	OnCurrentHPChanged.Broadcast(Percentage);
}

void UStatusComponent::OnRep_BurstMeter()
{
	float Percentage = FMath::Clamp(BurstMeter / MaxBurstMeter,0.f,1.f);
	//UE_LOG(LogTemp,Log,TEXT("OnRep_BurstMeter: %0.1f"),Percentage);
	OnBurstMeterChanged.Broadcast(Percentage);
}

void UStatusComponent::OnRep_SuperMeter()
{
	float Percentage = FMath::Clamp(SuperMeter / MaxSuperMeter,0.f,1.f);
	//UE_LOG(LogTemp,Log,TEXT("OnRep_SuperMeter: %0.1f"),Percentage);
	OnSuperMeterChanged.Broadcast(Percentage);
}
