// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackCollisionData.h"
#include "UObject/Interface.h"
#include "DamageAble.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDamageAble : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CCFF_API IDamageAble
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser, FHitBoxData HitData);
	
	// virtual float ApplyDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator,
	// 	AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass, FHitBoxData HitData) = 0;
	//
	// virtual float TakeDamage(float DamageAmount, const struct FDamageEvent& DamageEvent,
	// 	class AController* EventInstigator, AActor* DamageCauser, FHitBoxData HitData) = 0;
};
