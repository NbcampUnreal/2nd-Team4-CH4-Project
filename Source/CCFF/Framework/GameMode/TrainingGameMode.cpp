#include "Framework/GameMode/TrainingGameMode.h"
#include "Framework/GameState/TrainingGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"


ATrainingGameMode::ATrainingGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RoundDuration = 60.0f;
	MaxHP = 3;
}

void ATrainingGameMode::StartTraining()
{
	UE_LOG(LogTemp, Log, TEXT("Training started for %f seconds."), RoundDuration);

	ATrainingGameState* TGameState = Cast<ATrainingGameState>(GameState);
	if (IsValid(TGameState))
	{
		TGameState->TotalDamage = 0.0f;
		TGameState->MaxCombo = 0;
		TGameState->DPS = 0.0f;
	}

	if (!TrainingHUD && HUDWidgetClass)
	{
		TrainingHUD = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (TrainingHUD)
		{
			TrainingHUD->AddToViewport();
		}
	}

	StartRound();
}

void ATrainingGameMode::EndTraining()
{
	UE_LOG(LogTemp, Log, TEXT("Training ended."));
	EndRound();

	ATrainingGameState* TGameState = Cast<ATrainingGameState>(GameState);
	if (IsValid(TGameState))
	{
		TGameState->CalculateDPS(RoundDuration);
	}
}

void ATrainingGameMode::CheckGameConditions()
{
	EndTraining();
}
