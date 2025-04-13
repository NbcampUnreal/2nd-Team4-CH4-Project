#include "Framework/GameMode/ArenaGameMode.h"
#include "Framework/GameState/ArenaGameState.h"
#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/CountdownWidget.h"
#include "Algo/Sort.h"


AArenaGameMode::AArenaGameMode(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
	, DamageWeight(0.6f)
	, TimeWeight(0.4f)
{
	GameStateClass = AArenaGameState::StaticClass();
	PlayerStateClass = AArenaPlayerState::StaticClass();

	MyClassName = "ArenaMode";
	RoundTime = 10.0f;  // Default
	CountdownTime = 5.0f;
}

void AArenaGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (bHasGameStarted)
	{
		ErrorMessage = TEXT("Server is already in-game. Please try again later.");
	}
}

void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->InitializeGameState();
		ArenaGameState->SetCountdownTime(CountdownTime);
		ArenaGameState->SetRoundStartTime(RoundTime);
		ArenaGameState->SetRemainingTime(RoundTime);
	}

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AArenaGameMode::UpdateCountdown, 1.0f, true);
}

void AArenaGameMode::StartArenaRound()
{
	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->SetRoundProgress(ERoundProgress::InProgress);
		bHasGameStarted = true;
	}

	// CheckCondition every second
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ABaseInGameMode::CheckGameConditions, 1.0f, true);

	GetWorld()->GetTimerManager().SetTimer(ArenaTimerHandle, this, &AArenaGameMode::UpdateArenaStats, 1.0f, true);
}

void AArenaGameMode::EndRound()
{
	Super::EndRound();

	GetWorld()->GetTimerManager().ClearTimer(ArenaTimerHandle);
	RoundTime = 0.0f;

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->SetRemainingTime(RoundTime);
		ArenaGameState->SetRoundProgress(ERoundProgress::Ended);
		UpdatePlayerRating();
	}
} 

void AArenaGameMode::CheckGameConditions()
{
	Super::CheckGameConditions();

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (ArenaGameState)
	{
		if (ArenaGameState->GetRemainingTime() <= 0.0f)
		{
			EndRound();
		}
	}
	// TODO :: 남은 플레이어 수 1명 이상인지 확인
	//AArenaPlayerState* ArenaPlayerState = Cast <AArenaPlayerState>(PlayerState);
}

void AArenaGameMode::UpdateArenaStats()
{
	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		float CurrentRemainingTime = ArenaGameState->GetRemainingTime();
		ArenaGameState->SetRemainingTime(CurrentRemainingTime - 1.0f);
	}
}

void AArenaGameMode::UpdatePlayerRating()
{
	TArray<AArenaPlayerState*> RankingPlayers;
	if (GameState)
	{
		for (APlayerState* PS : GameState->PlayerArray)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState found: %s"), *PS->GetPlayerName());

			AArenaPlayerState* ArenaPS = Cast<AArenaPlayerState>(PS);
			if (IsValid(ArenaPS))
			{
				RankingPlayers.Add(ArenaPS);
			}
		}
	}

	Algo::Sort(RankingPlayers, [this](const AArenaPlayerState* A, const AArenaPlayerState* B)
		{
			const float ScoreA = (A->GetTotalDamage() * this->DamageWeight) + (A->GetSurvivalTime() * this->TimeWeight);
			const float ScoreB = (B->GetTotalDamage() * this->DamageWeight) + (B->GetSurvivalTime() * this->TimeWeight);
			return ScoreA > ScoreB;
		});

	TArray<FArenaRankInfo> RankingInfos;
	for (int32 Index = 0; Index < RankingPlayers.Num(); Index++)
	{
		FArenaRankInfo RankInfo;
		RankInfo.Rank = Index + 1;
		RankInfo.PlayerName = RankingPlayers[Index]->GetPlayerName();
		RankInfo.TotalDamage = RankingPlayers[Index]->GetTotalDamage();
		RankInfo.SurvivalTime = RankingPlayers[Index]->GetSurvivalTime();
		RankingInfos.Add(RankInfo);
		UE_LOG(LogTemp, Log, TEXT("%d. %s - TotalDamage: %.2f, SurvivalTime: %.2f"),
			Index + 1,
			*RankingPlayers[Index]->GetPlayerName(),
			RankingPlayers[Index]->GetTotalDamage(),
			RankingPlayers[Index]->GetSurvivalTime());
	}

	if (AArenaGameState* ArenaGS = Cast<AArenaGameState>(GameState))
	{
		ArenaGS->SetRankingInfos(RankingInfos);
	}
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


