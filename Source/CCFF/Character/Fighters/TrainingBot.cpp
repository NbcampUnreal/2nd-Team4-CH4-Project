// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Fighters/TrainingBot.h"

ATrainingBot::ATrainingBot()
{
	CharacterType="TrainingBot";
}

void ATrainingBot::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATrainingBot::BeginPlay()
{
	Super::BeginPlay();
}
