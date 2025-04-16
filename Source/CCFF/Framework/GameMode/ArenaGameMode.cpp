﻿#include "Framework/GameMode/ArenaGameMode.h"
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
#include "Items/Manager/ItemManager.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Character/Base/BaseCharacter.h"


AArenaGameMode::AArenaGameMode()
	: SelectedArenaSubMode(EArenaSubMode::Elimination)
	, DamageWeight(0.4f)
	, TimeWeight(0.2f)
	, KillCountWeight(0.4f)
{
	GameStateClass = AArenaGameState::StaticClass();
	PlayerControllerClass = ACharacterController::StaticClass();
	PlayerStateClass = AArenaPlayerState::StaticClass();

	MyClassName = "ArenaMode";
	RoundTime = 70.0f;  // Default
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

//void AArenaGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//	if (NewPlayer)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("+++++++++++++++++++++++++   SpawnPlayer"));
//		SpawnPlayer(NewPlayer);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("+++++++++++++++++++++++++   No PlayerContoller"));
//	}
//}


void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();

	UCCFFGameInstance* CCFFGameInstance = Cast<UCCFFGameInstance>(GetGameInstance());
	if (CCFFGameInstance)
	{
		SelectedArenaSubMode = CCFFGameInstance->GetArenaSubMode();
		UE_LOG(LogTemp, Log, TEXT("+++++++++++++++++++++++++++++++++ Selected ArenaSubMode: %d"), (uint8)SelectedArenaSubMode);

	}

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
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

		ArenaGameState->InitializeGameState();
		ArenaGameState->SetCountdownTime(CountdownTime);
		ArenaGameState->SetRoundStartTime(RoundTime);
		ArenaGameState->SetRemainingTime(RoundTime);
		ArenaGameState->SetArenaSubMode(SelectedArenaSubMode);
		ArenaGameState->SetSpectatorCamera(SpectatorCamera);
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

	ResetSubsystem();

	// CheckCondition every second
	GetWorld()->GetTimerManager().SetTimer(ConditionCheckTimerHandle, this, &AArenaGameMode::CheckGameConditions, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(ArenaTimerHandle, this, &AArenaGameMode::UpdateArenaStats, 1.0f, true);
}

void AArenaGameMode::SpawnPlayer(APlayerController* NewPlayer)
{
	AArenaPlayerState* PS = NewPlayer->GetPlayerState<AArenaPlayerState>();
	if (!PS) return;

	FName SelectedID = PS->GetSelectedCharacterID();

	TSubclassOf<ABaseCharacter> CharacterClass = CharacterClasses[SelectedID];

	AActor* StartSpot = ChoosePlayerStart(NewPlayer);
	const FTransform StartTransform = StartSpot
		? StartSpot->GetActorTransform()
		: FTransform::Identity;

	FActorSpawnParameters Params;
	Params.Owner = NewPlayer;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABaseCharacter* NewCharacter = GetWorld()->SpawnActor<ABaseCharacter>(
		CharacterClass,
		StartTransform.GetLocation(),
		StartTransform.GetRotation().Rotator(),
		Params
	);

	if (!NewCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("+++++++++++++++++++++++ [SpawnPlayer] Spawn failed: %s"), *CharacterClass->GetName());
		return;
	}

	// Possess
	NewPlayer->Possess(NewCharacter);
	if (ACharacterController* PC = Cast<ACharacterController>(NewPlayer))
	{
		PC->SetControlRotation(PC->GetControlRotation());

		UCharacterCustomizationComponent* CustomizationComponent = NewCharacter->FindComponentByClass<UCharacterCustomizationComponent>();
		if (CustomizationComponent)
		{
			int32 PresetIndex = PC->GetPlayerState<AArenaPlayerState>()->GetSelectedPresetIndex();
			UE_LOG(LogTemp, Log, TEXT("[GameMode] PresetIndex: %d"), PresetIndex);
			CustomizationComponent->EquipPresetByIndex(PresetIndex);
		}
	}	
}

void AArenaGameMode::EndRound()
{
	Super::EndRound();

	GetWorld()->GetTimerManager().ClearTimer(ConditionCheckTimerHandle);
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
	if (!bHasGameStarted)
	{
		return;
	}

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (ArenaGameState)
	{
		if (ArenaGameState->GetRemainingTime() <= 0.0f)
		{
			EndRound();
			return;
		}
	}

	if (SelectedArenaSubMode == EArenaSubMode::Elimination)
	{
		int32 AliveCount = 0;
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PlayerController = It->Get())
			{
				if (APawn* Pawn = PlayerController->GetPawn())
				{
					AliveCount++;
				}
				else
				{
					if (AArenaPlayerState * ArenaPlayerState = Cast<AArenaPlayerState>(PlayerController->PlayerState))
					 {
						if (ArenaPlayerState->MaxLives >= 0)
						{
							AliveCount++;
						}
					 }
				}
			}
		}

		if (AliveCount <= 1)
		{
			UE_LOG(LogTemp, Log, TEXT("+++++++++++++++++++++++ [ArenaGameMode Only Alive ] : AliveCount = %d"), AliveCount);
			EndRound();
			return;
		}
	}
}

void AArenaGameMode::ResetSubsystem()
{
	Super::ResetSubsystem();
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
