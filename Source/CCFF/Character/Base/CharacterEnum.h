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
	Dodge		UMETA(DisplayName = "Dodge"),
	AMove		UMETA(DisplayName = "AMove"),
	BMove		UMETA(DisplayName = "BMove"),
	CMove		UMETA(DisplayName = "CMove"),
	Grab		UMETA(DisplayName = "Grab"),
	Grabbed		UMETA(DisplayName = "Grabbed"),
	Burst		UMETA(DisplayName = "Burst"),
	Super		UMETA(DisplayName = "Super")
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
