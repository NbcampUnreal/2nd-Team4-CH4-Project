#include "Framework/GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "Framework/GameMode/LobbyGameMode.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/Controller/LobbyPlayerController.h"

void ALobbyGameState::UpdateAllowStartGame()
{
	const bool bNewAllowStart = EvaluateStartCondition();

	if (bAllowStartGame != bNewAllowStart)
	{
		bAllowStartGame = bNewAllowStart;
	}

	if (!bNewAllowStart)
	{
		UE_LOG(LogTemp, Log, TEXT("[LobbyGameState] All Ready ¡æ Not Ready : Countdown reset"));
		ResetCountdown();
	}
}

bool ALobbyGameState::EvaluateStartCondition() const
{
	int32 ReadyCount = GetReadyPlayerCount();
	int32 TotalCount = GetPlayerCount();

	if (ALobbyGameMode* GM = GetWorld()->GetAuthGameMode<ALobbyGameMode>())
	{
		// Allow solo start only if flag is set
		if (GM->bAllowSoloStart && TotalCount == 1 && ReadyCount == 1)
		{
			return true;
		}
	}

	return TotalCount >= 2 && ReadyCount == TotalCount;
}

int32 ALobbyGameState::GetPlayerCount() const
{
	return PlayerArray.Num();
}

int32 ALobbyGameState::GetReadyPlayerCount() const
{
	int32 ReadyCount = 0;

	for (APlayerState* PlayerState : PlayerArray)
	{
		const ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
		if (IsValid(LobbyPlayerState) && LobbyPlayerState->IsReady())
		{
			ReadyCount++;
		}
	}

	return ReadyCount;
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

void ALobbyGameState::ResetCountdown()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(CountdownTickHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTickHandle);
	}

	RemainingCountdownTime = -1;
	OnRep_RemainingCountdownTime();
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
