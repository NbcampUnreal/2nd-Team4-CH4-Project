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
#include "GameFramework/PlayerStart.h"
#include "Framework/Controller/CharacterController.h"


AArenaGameMode::AArenaGameMode(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
	, DamageWeight(0.4f)
	, TimeWeight(0.2f)
	, KillCountWeight(0.4f)
{
	PlayerControllerClass = ACharacterController::StaticClass();
	GameStateClass = AArenaGameState::StaticClass();
	PlayerStateClass = AArenaPlayerState::StaticClass();

	MyClassName = "ArenaMode";
	RoundTime = 30.0f;  // Default
	CountdownTime = 5.0f;
}

void AArenaGameMode::SpawnPlayer(AController* NewPlayer)
{
	if (!NewPlayer) return;

	FName SelectedID = NAME_None;
	if (AArenaPlayerState* ArenaPlayerState = NewPlayer->GetPlayerState<AArenaPlayerState>())
	{
		SelectedID = ArenaPlayerState->GetSelectedCharacterID();
		UE_LOG(LogTemp, Log, TEXT("++++++++++++++++++[SpawnPlayer] SelectedID: %s"), *SelectedID.ToString());
	}

	if (!CharacterClasses.Contains(SelectedID))
	{
		UE_LOG(LogTemp, Warning, TEXT("++++++++++++++++++[SpawnPlayer] '%s' no mapping"), *SelectedID.ToString());
		return;
	}
	TSubclassOf<APawn> PawnClass = CharacterClasses[SelectedID];

	AActor* StartSpot = ChoosePlayerStart(NewPlayer);
	const FTransform StartTransform = StartSpot ? StartSpot->GetActorTransform() : FTransform::Identity;
	const FVector SpawnLocation = StartTransform.GetLocation();
	const float SpawnYaw = StartTransform.GetRotation().Rotator().Yaw;

	const FRotator SpawnRotation(0.f, SpawnYaw, 0.f);

	FActorSpawnParameters Params;
	Params.Owner = NewPlayer;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnLocation, SpawnRotation, Params);
	if (NewPawn)
	{
		NewPlayer->Possess(NewPawn);

		if (APlayerController* PC = Cast<APlayerController>(NewPlayer))
		{
			FRotator CR = PC->GetControlRotation();
			PC->SetControlRotation(CR);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("++++++++++++++++++[SpawnPlayer] Spawn Fail : %s"), *PawnClass->GetName());
	}
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
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (AController* PlayerController = Cast<AController>(PlayerState->GetOwner()))
		{
			SpawnPlayer(PlayerController);
		}
	}

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->SetRoundProgress(ERoundProgress::InProgress);
		bHasGameStarted = true;
	}

	// CheckCondition every second
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &AArenaGameMode::CheckGameConditions, 1.0f, true);

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
	
	/*if (!bIsDeathmatch)
	{
		int32 AliveCount = 0;
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				if (APawn* Pawn = PC->GetPawn())
				{
					AliveCount++;
					UE_LOG(LogTemp, Log, TEXT("AliveCount : %d"), AliveCount);
				}
			}
		}

		if (AliveCount <= 1)
		{
			EndRound();
			return;
		}
	}*/
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
		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			if (AArenaPlayerState* ArenaPlayerState = Cast<AArenaPlayerState>(PlayerState))
			{
				RankingPlayers.Add(ArenaPlayerState);
			}
		}
	}

	Algo::Sort(RankingPlayers, [this](const AArenaPlayerState* A, const AArenaPlayerState* B)
		{
			const float ScoreA = (A->GetKillCount() * KillCountWeight) + (A->GetTotalDamage() * DamageWeight) + (A->GetSurvivalTime() * TimeWeight);
			const float ScoreB = (B->GetKillCount() * KillCountWeight) + (B->GetTotalDamage() * DamageWeight) + (B->GetSurvivalTime() * TimeWeight);
			return ScoreA > ScoreB;
		});

	TArray<FArenaRankInfo> RankingInfos;
	RankingInfos.Reserve(RankingPlayers.Num());

	for (int32 Index = 0; Index < RankingPlayers.Num(); Index++)
	{
		AArenaPlayerState* ArenaPlayerState = RankingPlayers[Index];
		FArenaRankInfo Info;

		Info.Rank = Index + 1;

		const FString NickName = ArenaPlayerState->GetPlayerNickname();
		if (!NickName.IsEmpty())
		{
			Info.PlayerName = RankingPlayers[Index]->GetPlayerName();
		}
		else
		{
			const FString PlayerName = ArenaPlayerState->GetPlayerName();
			Info.PlayerName = PlayerName;
		}

		Info.KillCount = ArenaPlayerState->GetKillCount();
		Info.TotalDamage = ArenaPlayerState->GetTotalDamage();
		Info.SurvivalTime = ArenaPlayerState->GetSurvivalTime();
		RankingInfos.Add(Info);
	}

	if (AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState))
	{
		ArenaGameState->SetRankingInfos(RankingInfos);
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

// TODO :: Player Respawn
void AArenaGameMode::RespawnPlayer(AController* Controller)
{
	if (Controller)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		if (PlayerStarts.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, PlayerStarts.Num() - 1);
			APlayerStart* SpawnPoint = Cast<APlayerStart>(PlayerStarts[Index]);
			if (SpawnPoint)
			{
				RestartPlayerAtPlayerStart(Controller, SpawnPoint);
				return;
			}
		}
		RestartPlayer(Controller);
	}
}


