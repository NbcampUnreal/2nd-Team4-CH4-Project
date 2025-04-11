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
	MaxHP=100;
	CurrentHP=MaxHP;
	SuperMeter=0;
	MaxSuperMeter=100;
	BurstMeter=0;
	MaxBurstMeter=100;
	//Set Replicate
	SetIsReplicatedByDefault(true);
}

void UStatusComponent::SetCurrentHP(float InCurrentHP)
{
	CurrentHP = InCurrentHP;
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnDeath.Broadcast();
	}
	OnCurrentHPChanged.Broadcast(CurrentHP);
}



void UStatusComponent::SetMaxHP(float InMaxHP)
{
	MaxHP = InMaxHP;
	if (MaxHP < KINDA_SMALL_NUMBER)
	{
		MaxHP = 0.1f;
	}
}

void UStatusComponent::SetBurstMeter(float InBurstMeter)
{
}

void UStatusComponent::SetSuperMeter(float InSuperMeter)
{
}

void UStatusComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass,CurrentHP);
	DOREPLIFETIME(ThisClass,SuperMeter);
	DOREPLIFETIME(ThisClass,BurstMeter);
}

void UStatusComponent::OnRep_CurrentHP()
{
	OnCurrentHPChanged.Broadcast(CurrentHP);
}

void UStatusComponent::OnRep_BurstMeter()
{
	OnBurstMeterChanged.Broadcast(BurstMeter);
}

void UStatusComponent::OnRep_SuperMeter()
{
	OnSuperMeterChanged.Broadcast(SuperMeter);
}
