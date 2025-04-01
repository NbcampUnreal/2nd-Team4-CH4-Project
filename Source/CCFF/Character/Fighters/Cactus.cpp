// Fill out your copyright notice in the Description page of Project Settings.


#include "Cactus.h"
#include "Character/DataLoaderSubSystem.h"

// Sets default values
ACactus::ACactus()
{
}

// Called when the game starts or when spawned
void ACactus::BeginPlay()
{
	Super::BeginPlay();
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			Stats=Loader->InitializeStat("Cactus");
		}
	}
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

