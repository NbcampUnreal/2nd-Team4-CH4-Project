#include "Framework/GameMode/ArenaGameMode.h"
#include "Framework/GameState/ArenaGameState.h"
#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/CountdownWidget.h"
#include "Algo/Sort.h"
#include "GameFramework/Pawn.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"


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

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpectatorCamera"), Found);
	if (Found.Num() > 0)
	{
		SpectatorCamera = Cast<ACameraActor>(Found[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ArenaGameMode: SpectatorCam not found"));
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
		
		float Initial = ArenaGameState->GetRoundStartTime();

		for (APlayerState* BasePlayerState : GameState->PlayerArray)
		{
			AArenaPlayerState* ArenaPlayerState = Cast<AArenaPlayerState>(BasePlayerState);
			if (ArenaPlayerState && ArenaPlayerState->GetSurvivalTime() <= 0.0f)
			{
				ArenaPlayerState->SetSurvivalTime(Initial);
			}
		}
	}
	UpdatePlayerRating();
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
			return;
		}
	}
	// TODO :: 남은 플레이어 수 1명 이상인지 확인
	//int32 AliveCount = 0;
	//for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	//{
	//	if (APlayerController* PC = It->Get())
	//	{
	//		if (APawn* Pawn = PC->GetPawn())
	//		{
	//			// Pawn이 아직 살아있다면 카운트
	//			if (IsValid(Pawn))
	//			{
	//				AliveCount++;
	//			}
	//		}
	//	}
	//}

	//if (AliveCount == 0)
	//{
	//	EndRound();
	//	return;
	//}
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
			if (AArenaPlayerState* ArenaPS = Cast<AArenaPlayerState>(PS))
			{
				RankingPlayers.Add(ArenaPS);
			}
		}
	}

	Algo::Sort(RankingPlayers, [this](const AArenaPlayerState* A, const AArenaPlayerState* B)
		{
			const float ScoreA = (A->GetTotalDamage() * DamageWeight) + (A->GetSurvivalTime() * TimeWeight);
			const float ScoreB = (B->GetTotalDamage() * DamageWeight) + (B->GetSurvivalTime() * TimeWeight);
			return ScoreA > ScoreB;
		});

	TArray<FArenaRankInfo> RankingInfos;
	RankingInfos.Reserve(RankingPlayers.Num());

	for (int32 Index = 0; Index < RankingPlayers.Num(); ++Index)
	{
		AArenaPlayerState* ArenaPlayerState = RankingPlayers[Index];
		FArenaRankInfo Info;
		Info.Rank = Index + 1;

		const FString NickName = ArenaPlayerState->GetPlayerNickname();
		if (!NickName.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Using PlayerNickname: %s"), *NickName);
			//Info.PlayerName = NickName;
			Info.PlayerName = RankingPlayers[Index]->GetPlayerName();
		}
		else
		{
			const FString PSName = ArenaPlayerState->GetPlayerName();
			UE_LOG(LogTemp, Warning, TEXT("PlayerNickname is empty, falling back to PlayerState Name: %s"), *PSName);
			Info.PlayerName = PSName;
		}

		Info.TotalDamage = ArenaPlayerState->GetTotalDamage();
		Info.SurvivalTime = ArenaPlayerState->GetSurvivalTime();
		RankingInfos.Add(Info);
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


