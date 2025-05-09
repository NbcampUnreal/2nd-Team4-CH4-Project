﻿#include "Framework/GameMode/BaseInGameMode.h"
#include "Framework/GameState/BaseInGameState.h"
#include "Items/Manager/ItemManager.h"

ABaseInGameMode::ABaseInGameMode()
{
	GameStateClass = ABaseInGameState::StaticClass();

	MyClassName = "BaseGameMode";
	RoundTime = 60.0f;  // Default Value
}

void ABaseInGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartRound();
	ResetSubsystem();
}

void ABaseInGameMode::ResetSubsystem()
{
	UItemManager* ItemManager = GetGameInstance()->GetSubsystem<UItemManager>();
	if (ItemManager)
	{
		UE_LOG(LogTemp, Log, TEXT("++++++++++++++++++++++++++++++++++++++++++++++++++++[BaseInGameMode initialize ResetSubsystem]"));
		ItemManager->InitializeItemManager();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("++++++++++++++++++++++++++++++++++++++++++++++++++++[BaseInGameMode not initialized ResetSubsystem]"));
		UE_LOG(LogTemp, Error, TEXT("ItemManager is not initialized!"));
	}
}

void ABaseInGameMode::StartRound()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Start Round!"), *MyClassName);

	if (ABaseInGameState* BGameState = GetGameState<ABaseInGameState>())
	{
		BGameState->InitializeGameState();
		BGameState->RoundProgress = ERoundProgress::InProgress;
	}

	//GetWorldTimerManager().SetTimer(
	//	GameTimerHandle,
	//	this,
	//	&ABaseInGameMode::CheckGameConditions,
	//	1.0f,
	//	true
	//);
}

void ABaseInGameMode::EndRound()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] End Round!"), *MyClassName);

	GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
	
	if (ABaseInGameState* BGameState = GetGameState<ABaseInGameState>())
	{
		BGameState->RoundProgress = ERoundProgress::Ended;
	}
}

void ABaseInGameMode::CheckGameConditions()
{

}
