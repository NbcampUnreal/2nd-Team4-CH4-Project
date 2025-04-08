#include "Framework/GameState/BaseInGameState.h"
#include "Net/UnrealNetwork.h"

ABaseInGameState::ABaseInGameState()
{
}

void ABaseInGameState::InitializeGameState()
{
	RoundProgress = ERoundProgress::NotStarted;
	RemainingTime = 0.0f;
}

void ABaseInGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, RoundProgress);
}

