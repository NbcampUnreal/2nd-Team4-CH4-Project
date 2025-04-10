#include "Framework/GameState/ArenaGameState.h"
#include "Net/UnrealNetwork.h"


AArenaGameState::AArenaGameState()
	: TotalDamage(0.0f),
	  CountdownTime(0.0f)
{
}

void AArenaGameState::InitializeGameState()
{
	Super::InitializeGameState();

	TotalDamage = 0.0f;
	CountdownTime = 3.0f;
}

void AArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaGameState, RemainingTime);
	DOREPLIFETIME(AArenaGameState, RoundProgress);
	DOREPLIFETIME(AArenaGameState, CountdownTime);
}