#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

ALobbyPlayerState::ALobbyPlayerState()
{
	bReplicates = true;
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, PlayerNickname);
	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}

void ALobbyPlayerState::SetReady(bool bNewReady)
{
	if (HasAuthority())
	{
		bIsReady = bNewReady;
	}
}

void ALobbyPlayerState::OnRep_ReadyState()
{
	// TODO: Add state to UI
	UE_LOG(LogTemp, Log, TEXT("Player [%s] Ready Status Changed: %s"), *PlayerNickname, bIsReady ? TEXT("Ready") : TEXT("Not Ready"));
}
