#include "Framework/GameMode/ArenaGameMode.h"
#include "Framework/GameState/ArenaGameState.h"
#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/CountdownWidget.h"


AArenaGameMode::AArenaGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = AArenaGameState::StaticClass();

	MyClassName = "ArenaMode";
	RoundTime = 10.0f;  // Default
	CountdownTime = 5.0f;
}

void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->InitializeGameState();
		ArenaGameState->SetCountdownTime(CountdownTime);
		ArenaGameState->SetRemainingTime(RoundTime);
	}
		

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AArenaGameMode::UpdateCountdown, 1.0f, true);
}

void AArenaGameMode::StartArenaRound()
{
	if (!HasAuthority()) { return; }

	//Super::StartRound();

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->SetRoundProgress(ERoundProgress::InProgress);
	}

	// CheckCondition every second
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ABaseInGameMode::CheckGameConditions, 1.0f, true);

	GetWorld()->GetTimerManager().SetTimer(ArenaTimerHandle, this, &AArenaGameMode::UpdateArenaStats, 1.0f, true);
}

void AArenaGameMode::EndRound()
{
	if (!HasAuthority()) return;

	Super::EndRound();

	GetWorld()->GetTimerManager().ClearTimer(ArenaTimerHandle);
	RoundTime = 0.0f;

	UpdatePlayerRating();

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->SetRemainingTime(RoundTime);
		ArenaGameState->SetRoundProgress(ERoundProgress::Ended);
	}

    // Move Result Map
	GetWorld()->GetTimerManager().SetTimer(
		LevelTransitionTimerHandle,
		this,
		&AArenaGameMode::MoveResultLevel,
		1.0f,
		false
	);
} 

void AArenaGameMode::CheckGameConditions()
{
	if (!HasAuthority()) { return; }

	Super::CheckGameConditions();

	// TODO :: 남은 플레이어 1명일 때 종료
	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (ArenaGameState)
	{
		if (ArenaGameState->GetRemainingTime() <= 0.0f)
		{
			EndRound();
		}
		// TODO :: Get PlayerState RemainPlayer Or Other Conditions
	}
}

void AArenaGameMode::UpdateArenaStats()
{
	if (!HasAuthority()) return;

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		float CurrentRemainingTime = ArenaGameState->GetRemainingTime();
		ArenaGameState->SetRemainingTime(CurrentRemainingTime - 1.0f);
	}
}

void AArenaGameMode::UpdatePlayerRating()
{
}

void AArenaGameMode::UpdateCountdown()
{
	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (!IsValid(ArenaGameState))
	{
		return;
	}

	ArenaGameState->CountdownTime = CountdownTime;

	if (CountdownTime <= 0.0f)
	{
		ArenaGameState->bIsFinishCountdown = true;
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AArenaGameMode::StartArenaRound, 0.1f, false);
		return;
	}

	CountdownTime -= 1.0f;
	ArenaGameState->CountdownTime = CountdownTime;
}

void AArenaGameMode::MoveResultLevel()
{
	if (!HasAuthority()) return;

	GetWorld()->ServerTravel(TEXT("/Game/CCFF/Maps/LobbyMap?Listen"));
}



