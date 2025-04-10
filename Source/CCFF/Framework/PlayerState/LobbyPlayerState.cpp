#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

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
	UE_LOG(LogTemp, Log, TEXT("Player [%s] Ready Status Changed: %s"), *PlayerNickname, bIsReady ? TEXT("Ready") : TEXT("Not Ready"));
	OnReadyStateChanged.Broadcast(bIsReady);
}

void ALobbyPlayerState::OnRep_PlayerNickname()
{
	UE_LOG(LogTemp, Log, TEXT("[LobbyPlayerState] OnRep_PlayerNickname: %s"), *PlayerNickname);
	OnNicknameUpdated.Broadcast();
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, PlayerNickname);
	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}
