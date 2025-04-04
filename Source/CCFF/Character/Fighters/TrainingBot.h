// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/BaseCharacter.h"
#include "TrainingBot.generated.h"

/**
 * 
 */
UCLASS()
class CCFF_API ATrainingBot : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ATrainingBot();

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
