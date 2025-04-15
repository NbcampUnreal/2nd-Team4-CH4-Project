#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Net/UnrealNetwork.h"

AArenaPlayerState::AArenaPlayerState()
	: PlayerNickname(TEXT("")),
	  KillCount(0),
	  TotalDamage(0.0f),
	  SurvivalTime(0.0f),
	  MaxLives(3),
	  MaxHP(1000.0f)
{
	bReplicates = true;
}

void AArenaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaPlayerState, PlayerNickname);
	DOREPLIFETIME(AArenaPlayerState, TotalDamage);
	DOREPLIFETIME(AArenaPlayerState, SurvivalTime);
	DOREPLIFETIME(AArenaPlayerState, MaxLives);
	DOREPLIFETIME(AArenaPlayerState, KillCount);
	DOREPLIFETIME(AArenaPlayerState, SelectedCharacterID);
	DOREPLIFETIME(AArenaPlayerState, MaxHP);
}
