#include "Framework/GameState/BaseInGameState.h"
#include "Net/UnrealNetwork.h"

ABaseInGameState::ABaseInGameState()
{
}

void ABaseInGameState::InitializeGameState()
{
	RoundProgress = ERoundProgress::NotStarted;
}

