#include "Framework/GameState/TrainingGameState.h"
#include "Net/UnrealNetwork.h"

ATrainingGameState::ATrainingGameState()
	: RemainingTime(0.f),
	  TotalDamage(0.f),
	  DPS(0.f)
{
}

void ATrainingGameState::InitializeGameState()
{
	Super::InitializeGameState();

	TotalDamage = 0.0f;
	DPS = 0.0f;
}
