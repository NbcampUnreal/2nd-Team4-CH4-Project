// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AttackCollisionData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	Box				UMETA(DisplayName = "Box"),
	Sphere			UMETA(DisplayName = "Sphere"),
	Capsule			UMETA(DisplayName = "Capsule"),
};

UENUM(BlueprintType)
enum class EHitBoxType : uint8
{
	Normal			UMETA(DisplayName = "Normal"),
	Throw			UMETA(DisplayName = "Throw"),
	Burst			UMETA(DisplayName = "Burst"),
	Projectile		UMETA(DisplayName = "Projectile"),
};

USTRUCT(BlueprintType)
struct FAttackCollisionData :public FTableRowBase
{
	GENERATED_BODY();
public:
	FAttackCollisionData(): Shape(EAttackCollisionType::Box),Location(0.0f),Rotation(0.0f),Scale(0.0f){}

	// Collision type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	EAttackCollisionType Shape;

	// Transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FVector Scale;
};

USTRUCT(BlueprintType)
struct FHitBoxData : public FTableRowBase
{
	GENERATED_BODY();
public:
	FHitBoxData(): Type(EHitBoxType::Normal), Damage(0.0f), MinimumDamage(0.0f), GuardDamage(0.0f), GuardMeterDamage(0.0f),Hitstun(0.0f), Hitlag(0.0f),
					VictimHitlag(0.0f), Blockstun(0.0f), KnockbackForce(0.0f), KnockbackAngle(0.0f), DiModifier(0.0f), GuardPushback(0.0f) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	EHitBoxType Type;

	// Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MinimumDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float GuardDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float GuardMeterDamage;

	// HitReaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
	int32 Hitstun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
	int32 Hitlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
	int32 VictimHitlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
	int32 Blockstun;

	//Knockback
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
	float KnockbackForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
	FVector KnockbackAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
	float DiModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
	float GuardPushback;
};
