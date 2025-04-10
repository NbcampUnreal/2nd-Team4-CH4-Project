#include "Framework/GameState/TrainingGameState.h"
#include "Net/UnrealNetwork.h"

ATrainingGameState::ATrainingGameState()
	: TrainingRemainingTime(0.f),
	  TotalDamage(0.f),
	  DPS(0.f)
{
}

void ATrainingGameState::InitializeGameState()
{
	Super::InitializeGameState();

	TrainingRemainingTime = 0.0f;
	TotalDamage = 0.0f;
	DPS = 0.0f;
}
