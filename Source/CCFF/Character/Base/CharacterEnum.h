// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "CharacterEnum.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Hitted		UMETA(DisplayName = "Hitted"),
	Block 		UMETA(DisplayName = "Block"),
	Blockstun	UMETA(DisplayName = "Blockstun"),
	Dodge		UMETA(DisplayName = "Dodge"),
	Attack		UMETA(DisplayName = "Attack"),
	Grab		UMETA(DisplayName = "Grab"),
	Grabbed		UMETA(DisplayName = "Grabbed"),
	Dead		UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EResistanceState : uint8
{
	Normal					UMETA(DisplayName = "Normal"),
	HyperArmor				UMETA(DisplayName = "HyperArmor"),
	ProjectileArmor			UMETA(DisplayName = "ProjectileArmor"),
	Invulnerable			UMETA(DisplayName = "Invulnerable"),
	ProjectileInvulnerable	UMETA(DisplayName = "ProjectileInvulnerable"),
	ThrowInvulnerable		UMETA(DisplayName = "ThrowInvulnerable")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Attack1		UMETA(DisplayName="Attack1"),
	Attack2		UMETA(DisplayName="Attack2"),
	Attack3		UMETA(DisplayName="Attack3"),
	Attack4		UMETA(DisplayName="Attack4"),
	Attack5		UMETA(DisplayName="Attack5"),
	Attack6		UMETA(DisplayName="Attack6"),
	Attack7		UMETA(DisplayName="Attack7"),
	Attack8		UMETA(DisplayName="Attack8"),
	Attack9		UMETA(DisplayName="Attack9"),
	Attack10	UMETA(DisplayName="Attack10"),
	Attack11	UMETA(DisplayName="Attack11"),
	Attack12	UMETA(DisplayName="Attack12"),
	Attack13	UMETA(DisplayName="Attack13"),
	Attack14	UMETA(DisplayName="Attack14"),
	Attack15	UMETA(DisplayName="Attack15"),
	Attack16	UMETA(DisplayName="Attack16"),
	None		UMETA(DisplayName = "None")
};