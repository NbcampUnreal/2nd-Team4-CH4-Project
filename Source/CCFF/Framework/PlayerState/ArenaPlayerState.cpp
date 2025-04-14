#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Net/UnrealNetwork.h"

AArenaPlayerState::AArenaPlayerState()
	: PlayerNickname(TEXT("")),
	  TotalDamage(0.0f),
	  SurvivalTime(0.0f)
{
	bReplicates = true;
}

void AArenaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaPlayerState, PlayerNickname);
	DOREPLIFETIME(AArenaPlayerState, TotalDamage);
	DOREPLIFETIME(AArenaPlayerState, SurvivalTime);
}
