#include "Framework/GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/Controller/LobbyPlayerController.h"

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

void ALobbyGameState::OnRep_RemainingCountdownTime()
{
	UE_LOG(LogTemp, Log, TEXT("[LobbyGameState] Countdown: %d seconds left"), RemainingCountdownTime);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(It->Get()))
		{
			LobbyPlayerController->UpdateCountdownWidget(RemainingCountdownTime);
		}
	}
}

void ALobbyGameState::StartCountdownTimer()
{
	RemainingCountdownTime = 10;
	OnRep_RemainingCountdownTime();

	GetWorld()->GetTimerManager().SetTimer(
		CountdownTickHandle,
		this,
		&ALobbyGameState::TickCountdownTimer,
		1.0f,
		true
	);
}

void ALobbyGameState::TickCountdownTimer()
{
	if (--RemainingCountdownTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTickHandle);
	}

	OnRep_RemainingCountdownTime();
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, bAllowStartGame);
	DOREPLIFETIME(ALobbyGameState, RemainingCountdownTime);
}
