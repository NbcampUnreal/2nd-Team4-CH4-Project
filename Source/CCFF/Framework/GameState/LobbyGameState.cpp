#include "Framework/GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "Framework/PlayerState/LobbyPlayerState.h"

void ALobbyGameState::UpdateAllowStartGame()
{
	bAllowStartGame = AreAllPlayersReady();
}

bool ALobbyGameState::AreAllPlayersReady() const
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		const ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
		if (LobbyPlayerState && LobbyPlayerState->IsReady() == false)
		{
			return false;
		}
	}
	return true;
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, bAllowStartGame);
}
