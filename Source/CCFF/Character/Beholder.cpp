// Fill out your copyright notice in the Description page of Project Settings.


#include "Beholder.h"


// Sets default values
ABeholder::ABeholder()
{
	InitializeStat("Beholder");
}

// Called when the game starts or when spawned
void ABeholder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeholder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABeholder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

