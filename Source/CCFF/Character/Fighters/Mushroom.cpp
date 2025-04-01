// Fill out your copyright notice in the Description page of Project Settings.


#include "Mushroom.h"
#include "Character/DataLoaderSubSystem.h"

// Sets default values
AMushroom::AMushroom()
{
}

// Called when the game starts or when spawned
void AMushroom::BeginPlay()
{
	Super::BeginPlay();
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			Stats=Loader->InitializeStat("Mushroom");
		}
	}
}

// Called every frame
void AMushroom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMushroom::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

