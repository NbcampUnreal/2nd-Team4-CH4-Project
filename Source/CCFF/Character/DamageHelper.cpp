// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageHelper.h"
#include "Base/DamageAble.h"
#include "Engine/DamageEvents.h"

float UDamageHelper::ApplyDamage(AActor* DamagedActor,
	float BaseDamage,
	AController* EventInstigator,
	AActor* DamageCauser,
	TSubclassOf<UDamageType> DamageTypeClass,
	FHitBoxData& HitData)
{
	UE_LOG(LogTemp, Display, TEXT("ApplyDamage_Implementation"));
	if ( DamagedActor && (BaseDamage != 0.f) )
	{
		// make sure we have a good damage type
		TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		if (DamagedActor->Implements<UDamageAble>())
		{
			return IDamageAble::Execute_TakeDamage(DamagedActor, BaseDamage, DamageEvent, EventInstigator, DamageCauser,HitData);	
		}
	}

	return 0.f;
}
