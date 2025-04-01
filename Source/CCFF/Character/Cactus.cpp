// Fill out your copyright notice in the Description page of Project Settings.


#include "Cactus.h"


// Sets default values
ACactus::ACactus()
{
	InitializeStat("Cactus");
}

// Called when the game starts or when spawned
void ACactus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACactus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACactus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

