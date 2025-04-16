// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleShell.h"


// Sets default values
ATurtleShell::ATurtleShell()
{
	CharacterType="TurtleShell";
}

// Called when the game starts or when spawned
void ATurtleShell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurtleShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATurtleShell::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

