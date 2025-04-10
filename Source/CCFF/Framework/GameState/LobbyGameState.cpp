#include "Framework/GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "Framework/PlayerState/LobbyPlayerState.h"

void ALobbyGameState::UpdateAllowStartGame()
{
	const bool bNewAllowStart = EvaluateStartCondition();

	if (bAllowStartGame != bNewAllowStart)
	{
		bAllowStartGame = bNewAllowStart;
	}
}

bool ALobbyGameState::EvaluateStartCondition() const
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		const ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
		if (IsValid(LobbyPlayerState) && LobbyPlayerState->IsReady() == false)
		{
			return false;
		}
	}
	return true;
}

void ALobbyGameState::OnRep_bAllowStartGame()
{
	UE_LOG(LogTemp, Log, TEXT("[LobbyGameState] OnRep_bAllowStartGame: %s"), bAllowStartGame ? TEXT("true") : TEXT("false"));

	OnAllowStartGameChanged.Broadcast(bAllowStartGame);
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, bAllowStartGame);
}
