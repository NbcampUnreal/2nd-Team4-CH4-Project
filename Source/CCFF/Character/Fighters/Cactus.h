// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/BaseCharacter.h"
#include "Cactus.generated.h"

UCLASS()
class CCFF_API ACactus : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACactus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
