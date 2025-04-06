#include "Framework/GameMode/TrainingGameMode.h"
#include "Framework/GameState/TrainingGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"

ATrainingGameMode::ATrainingGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RoundDuration = 60.0f;
	MyClassName = "TrainingMode";
}

void ATrainingGameMode::StartRound()
{
	Super::StartRound();

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
}

void ATrainingGameMode::EndRound()
{
	Super::EndRound();

	ATrainingGameState* TGameState = Cast<ATrainingGameState>(GameState);
	if (IsValid(TGameState))
	{
		TGameState->CalculateDPS(RoundDuration);
	}
}

void ATrainingGameMode::CheckGameConditions()
{
	Super::CheckGameConditions();

	// Call EndRound() when condition is true
}
