#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

ALobbyPlayerState::ALobbyPlayerState()
{
	bReplicates = true;
	bIsReady = false;
	PlayerNickname = TEXT("Player");
}

void ALobbyPlayerState::SetReady(bool bNewReady)
{
	if (HasAuthority())
	{
		bIsReady = bNewReady;
		OnRep_ReadyState();
	}
}

void ALobbyPlayerState::SetPlayerNickname(const FString& InNickname)
{
	if (HasAuthority())
	{
		PlayerNickname = InNickname;
		OnRep_PlayerNickname();
	}
}

void ALobbyPlayerState::OnRep_ReadyState()
{
}

void ALobbyPlayerState::OnRep_PlayerNickname()
{
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, PlayerNickname);
	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}
