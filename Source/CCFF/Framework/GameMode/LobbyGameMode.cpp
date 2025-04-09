#include "Framework/GameMode/LobbyGameMode.h"
#include "Framework/GameState/LobbyGameState.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/Controller/LobbyPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
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

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 IndexToAssign = AssignedSlotIndices.Num();
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

	FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	RestartPlayerAtTransform(NewPlayer, SpawnTransform);
	UE_LOG(LogTemp, Log, TEXT("[ALobbyGameMode] PostLogin : Restarted player %s at slot %d"), *NewPlayer->GetName(), IndexToAssign);
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

	int32 RandomIndex = FMath::RandRange(0, AvailableMapPaths.Num() - 1);
	const FString& SelectedMap = AvailableMapPaths[RandomIndex];
	UE_LOG(LogTemp, Log, TEXT("[ALobbyGameMode] StartGame : All players ready. Starting game on map: %s"), *SelectedMap);

	GetWorld()->ServerTravel(SelectedMap + TEXT("?listen"));
}
