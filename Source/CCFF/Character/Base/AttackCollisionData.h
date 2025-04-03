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

USTRUCT(BlueprintType)
struct FAttackCollisionData :public FTableRowBase
{
	GENERATED_BODY();
public:
	FAttackCollisionData(): Shape(EAttackCollisionType::Box),Location(0.0f),Rotation(0.0f),Scale(0.0f) {}

	// Collision type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
	EAttackCollisionType Shape;

	// Transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
	FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
	FVector Scale;
	
};
