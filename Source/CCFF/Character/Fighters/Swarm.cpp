// Fill out your copyright notice in the Description page of Project Settings.


#include "Swarm.h"


// Sets default values
ASwarm::ASwarm()
{
	CharacterType="Swarm";
}

// Called when the game starts or when spawned
void ASwarm::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwarm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASwarm::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

