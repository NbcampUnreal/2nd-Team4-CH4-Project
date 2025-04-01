// Fill out your copyright notice in the Description page of Project Settings.


#include "Beholder.h"
#include "Character/DataLoaderSubSystem.h"


// Sets default values
ABeholder::ABeholder()
{
}

// Called when the game starts or when spawned
void ABeholder::BeginPlay()
{
	Super::BeginPlay();
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			Stats=Loader->InitializeStat("Beholder");
		}
	}
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

