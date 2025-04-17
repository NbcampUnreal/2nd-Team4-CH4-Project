#include "Framework/GameMode/ArenaGameMode.h"
#include "Framework/GameState/ArenaGameState.h"
#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/CountdownWidget.h"
#include "Framework/Controller/CharacterController.h"
#include "Character/Base/BaseCharacter.h"
#include "Items/Manager/ItemManager.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Algo/Sort.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerStart.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"


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

void AArenaGameMode::StartPlay()
{
	Super::StartPlay();

	if (auto GI = GetGameInstance<UCCFFGameInstance>())
	{
		SelectedArenaSubMode = GI->GetArenaSubMode();
		UE_LOG(LogTemp, Warning, TEXT("+++++++++++++++++++++++++++++++++ [ArenaGameMode::StartPlay] SubMode=%d"), (uint8)SelectedArenaSubMode);
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

	UE_LOG(LogTemp, Warning,
		TEXT("+++++++++++++++++++++++++++ [ArenaGameMode] GameInstance=%s, SpectatorCameraClass=%s"),
		*GetGameInstance()->GetClass()->GetName(),
		*GetNameSafe(SpectatorCameraClass));
	
	if (!HasAuthority() || !SpectatorCameraClass)
	{
		return;
	}

	/*if (UCCFFGameInstance* CCFFGI = GetGameInstance<UCCFFGameInstance>())
	{
		SelectedArenaSubMode = CCFFGI->GetArenaSubMode();
		UE_LOG(LogTemp, Warning,
			TEXT("+++++++++++++++++++++++++++ [ArenaGameMode] Loaded ArenaSubMode from GI: %d"),
			(uint8)SelectedArenaSubMode);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("+++++++++++++++++++++++++++ [ArenaGameMode] Failed to cast GameInstance to CCFFGameInstance!"));
	}*/

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpectatorCameraClass, FoundCameras);

	FTransform SpawnTransform = FTransform::Identity;
	if (FoundCameras.Num() > 0)
	{
		if (ACameraActor* LevelCam = Cast<ACameraActor>(FoundCameras[0]))
		{
			SpawnTransform = LevelCam->GetActorTransform();
			LevelCam->SetActorHiddenInGame(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("+++++++++++++  [ArenaGameMode] Not Found Level SpectatorCameraClass Instance"));
		const FVector DefaultLoc(-900.f, -1185.f, 10580.f);
		const FRotator DefaultRot(0.f, -90.f, 0.f);
		SpawnTransform = FTransform(DefaultRot, DefaultLoc);
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.bNoFail = true;

	ACameraActor* SpectatorCamera = GetWorld()->SpawnActor<ACameraActor>(
		SpectatorCameraClass,
		SpawnTransform,
		Params
	);

	if (IsValid(SpectatorCamera))
	{
		SpectatorCamera->SetReplicates(true);
		SpectatorCamera->bAlwaysRelevant = true;
		SpectatorCamera->bNetLoadOnClient = true;
		SpectatorCamera->SetActorHiddenInGame(false);
		SpectatorCamera->SetActorEnableCollision(false);

		if (AArenaGameState* GS = GetWorld()->GetGameState<AArenaGameState>())
		{
			GS->SetSpectatorCamera(SpectatorCamera);
		}
	}

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->InitializeGameState();
		ArenaGameState->SetCountdownTime(CountdownTime);
		ArenaGameState->SetRoundStartTime(RoundTime);
		ArenaGameState->SetRemainingTime(RoundTime);
		ArenaGameState->SetArenaSubMode(SelectedArenaSubMode);
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
	if (SelectedID==NAME_None) return;
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
	if (ACharacterController* PC = Cast<ACharacterController>(NewPlayer))
	{
		NewPlayer->Possess(NewCharacter);
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

	/*AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (ArenaGameState)
	{
		if (ArenaGameState->GetRemainingTime() <= 0.0f)
		{
			EndRound();
			return;
		}
	}*/
	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (!ArenaGameState) { return; }
	const float RemainingTime = ArenaGameState->GetRemainingTime();

	if (ArenaGameState->GetArenaSubMode() == EArenaSubMode::DeathMatch)
	{
		if (RemainingTime <= 0.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("++++++++++++++++++++++++++++++ [ArenaGameMode] DeathMatch time-up: Ending round"));
			EndRound();
		}
		return;
	}

	if (ArenaGameState->GetArenaSubMode() == EArenaSubMode::Elimination)
	{
		int32 PlayersWithLives = 0;
		for (APlayerState* BasePS : ArenaGameState->PlayerArray)
		{
			if (AArenaPlayerState* PS = Cast<AArenaPlayerState>(BasePS))
			{
				if (PS->MaxLives >= 0)
				{
					PlayersWithLives++;
				}
			}
		}

		UE_LOG(LogTemp, Log,
			TEXT("++++++++++++++++++++++++ [ArenaGameMode] Elimination Checking - PlayersWithLives : %d, RemainingTime: %.1f"),
			PlayersWithLives, RemainingTime);

		if (RemainingTime <= 0.0f || PlayersWithLives <= 1)
		{
			UE_LOG(LogTemp, Log, TEXT("++++++++++++++++++++++++ [ArenaGameMode] Elimination Fininsh GameEnd Condition"));
			EndRound();
		}
		return;
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
