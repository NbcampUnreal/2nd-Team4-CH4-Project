// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DamageHelper.generated.h"

/**
 * 
 */
struct FHitBoxData;
UCLASS()
class CCFF_API UDamageHelper : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="DamageHelper")
	static float ApplyDamage(
		AActor* DamagedActor,
		float BaseDamage,
		AController* EventInstigator,
		AActor* DamageCauser,
		TSubclassOf<UDamageType> DamageTypeClass,
		FHitBoxData& HitData);
};
