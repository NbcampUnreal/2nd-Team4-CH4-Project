// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageHelper.h"
#include "Base/DamageAble.h"
#include "Engine/DamageEvents.h"

float UDamageHelper::ApplyDamage(
	AActor* DamagedActor,
	float BaseDamage,
	AController* EventInstigator,
	AActor* DamageCauser,
	TSubclassOf<UDamageType> DamageTypeClass,
	FHitBoxData& HitData
	)
{
	if ( DamagedActor && (BaseDamage != 0.f) )
	{
		// make sure we have a good damage type
		TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		if (DamagedActor->Implements<UDamageAble>())
		{
			UE_LOG(LogTemp, Display, TEXT("ApplyDamage"));
			return IDamageAble::Execute_TakeDamage(DamagedActor, BaseDamage, DamageEvent, EventInstigator, DamageCauser,HitData);	
		}
	}

	return 0.f;
}

void UDamageHelper::MyPrintString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay, FColor InColor)
{
	if (IsValid(GEngine) == true && IsValid(InWorldContextActor) == true)
	{
		if (InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer)
		{
			GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
		}
	}
}

FString UDamageHelper::GetNetModeString(const AActor* InWorldContextActor)
{
	FString NetModeString = TEXT("None");

	if (IsValid(InWorldContextActor) == true)
	{
		ENetMode NetMode = InWorldContextActor->GetNetMode();
		if (NetMode == NM_Client)
		{
			NetModeString = TEXT("Client");
		}
		else
		{
			if (NetMode == NM_Standalone)
			{
				NetModeString = TEXT("StandAlone");
			}
			else
			{
				NetModeString = TEXT("Server");
			}
		}
	}
		
	return NetModeString;
}

FString UDamageHelper::GetRoleString(const AActor* InActor)
{
	FString RoleString = TEXT("None");

	if (IsValid(InActor) == true)
	{
		FString LocalRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetLocalRole());
		FString RemoteRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetRemoteRole());
			
		RoleString = FString::Printf(TEXT("%s / %s"), *LocalRoleString, *RemoteRoleString);
	}

	return RoleString;
}
