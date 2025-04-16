#include "Framework/GameMode/LobbyGameMode.h"
#include "Framework/GameState/LobbyGameState.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/Controller/LobbyPlayerController.h"
#include "Character/Lobby/LobbyPreviewPawn.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	DefaultPawnClass = nullptr;

	if (AvailableMapPaths.Num() == 0)
	{
		AvailableMapPaths = {
			TEXT("/Game/CCFF/Maps/DesertMap"),
			TEXT("/Game/CCFF/Maps/MushroomMap"),
			TEXT("/Game/CCFF/Maps/RiverMap")
		};
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < 4; i++)
	{
		FString TagName = FString::Printf(TEXT("Slot_%d"), i);
		FName Tag(*TagName);

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

		if (FoundActors.Num() > 0)
		{
			PlayerSpawnSlots.Add(FoundActors[0]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[ALobbyGameMode] BeginPlay : Spawn slot with tag [%s] not found"), *TagName);
		}
	}
}

void ALobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (GetNumPlayers() >= 4)
	{
		ErrorMessage = TEXT("[LobbyGameMode] PreLogin : The lobby is full. Please try again later.");
		return;
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const int32 IndexToAssign = AssignedSlotIndices.Num();
	AssignedSlotIndices.Add(NewPlayer, IndexToAssign);

	if (!PlayerSpawnSlots.IsValidIndex(IndexToAssign))
	{
		UE_LOG(LogTemp, Warning, TEXT("[LobbyGameMode] PostLogin : Not enough slot positions!"));
		return;
	}

	FVector SpawnLocation = PlayerSpawnSlots[IndexToAssign]->GetActorLocation();
	FRotator SpawnRotation = PlayerSpawnSlots[IndexToAssign]->GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = NewPlayer;

	ALobbyPreviewPawn* PreviewPawn = GetWorld()->SpawnActor<ALobbyPreviewPawn>(PreviewPawnClass, SpawnLocation, SpawnRotation, Params);
	if (!PreviewPawn) return;

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(NewPlayer);
	if (!IsValid(LobbyPlayerController)) return;

	LobbyPlayerController->Possess(PreviewPawn);
	LobbyPlayerController->SetLobbyCameraView();

	if (APlayerState* NewPlayerState = NewPlayer->GetPlayerState<APlayerState>())
	{
		if (GameState->PlayerArray.Num() == 1)
		{
			if (ALobbyPlayerState* LobbyState = Cast<ALobbyPlayerState>(NewPlayerState))
			{
				LobbyState->SetIsHost(true);
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	APlayerState* ExitingPlayerState = Exiting->GetPlayerState<APlayerState>();
	if (!IsValid(ExitingPlayerState)) return;

	ALobbyPlayerState* ExitingLobbyState = Cast<ALobbyPlayerState>(ExitingPlayerState);
	if (!IsValid(ExitingLobbyState)) return;

	if (ExitingLobbyState->GetIsHost())
	{
		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			if (ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState))
			{
				LobbyPlayerState->SetIsHost(false);
			}
		}

		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			if (ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState))
			{
				if (LobbyPlayerState != ExitingLobbyState)
				{
					LobbyPlayerState->SetIsHost(true);
					UE_LOG(LogTemp, Log, TEXT("[LobbyGameMode] Host left. New host assigned: %s"), *LobbyPlayerState->GetPlayerNickname());
					break;
				}
			}
		}
	}
}

void ALobbyGameMode::NotifyPlayerReadyStatusChanged()
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (LobbyGameState)
	{
		LobbyGameState->UpdateAllowStartGame();

		if (LobbyGameState->EvaluateStartCondition())
		{
			StartGameWithDelay();
		}
	}
}

void ALobbyGameMode::StartGameWithDelay()
{
	UE_LOG(LogTemp, Log, TEXT("[ALobbyGameMode] StartGameWithDelay : All players ready. Starting countdown..."));

	if (ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>())
	{
		LobbyGameState->StartCountdownTimer();
	}

	GetWorld()->GetTimerManager().SetTimer(
		GameStartTimerHandle,
		this,
		&ALobbyGameMode::StartGame,
		10.0f,
		false);
}

void ALobbyGameMode::StartGame()
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (LobbyGameState == nullptr || !LobbyGameState->EvaluateStartCondition())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyGameMode] StartGame : Cannot start game: Not all players are ready."));
		return;
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(It->Get()))
		{
			PC->ClientTeardownCountdown();
		}
	}

	int32 RandomIndex = FMath::RandRange(0, AvailableMapPaths.Num() - 1);
	const FString& SelectedMap = AvailableMapPaths[RandomIndex];
	UE_LOG(LogTemp, Log, TEXT("[ALobbyGameMode] StartGame : All players ready. Starting game on map: %s"), *SelectedMap);

	GetWorld()->ServerTravel(SelectedMap + TEXT("?listen"));
}
