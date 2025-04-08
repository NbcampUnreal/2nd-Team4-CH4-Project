#include "Framework/GameMode/LobbyGameMode.h"
#include "Framework/GameState/LobbyGameState.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/Controller/LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
	if (AvailableMapPaths.Num() == 0)
	{
		AvailableMapPaths = {
			TEXT("/Game/CCFF/Maps/LobbyMap"),
			TEXT("/Game/CCFF/Maps/LobbyMap"),
			TEXT("/Game/CCFF/Maps/LobbyMap")
		};
	}
}

void ALobbyGameMode::NotifyPlayerReadyStatusChanged()
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (LobbyGameState)
	{
		LobbyGameState->UpdateAllowStartGame();
	}
}

void ALobbyGameMode::StartGame()
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (LobbyGameState == nullptr || !LobbyGameState->AreAllPlayersReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyGameMode] StartGame : Cannot start game: Not all players are ready."));
		return;
	}

	const FString SelectedMap = SelectMapForMatch();;
	UE_LOG(LogTemp, Log, TEXT("All players ready. Starting game on map: %s"), *SelectedMap);

	GetWorld()->ServerTravel(SelectedMap + TEXT("?listen"));
}

FString ALobbyGameMode::SelectMapForMatch() const
{
	if (AvailableMapPaths.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No available battle maps found. Returning to lobby map."));
		return TEXT("/Game/CCFF/Maps/LobbyMap");
	}

	if (SelectedMapIndex >= 0 && AvailableMapPaths.IsValidIndex(SelectedMapIndex))
	{
		return AvailableMapPaths[SelectedMapIndex];
	}

	return AvailableMapPaths[FMath::RandRange(0, AvailableMapPaths.Num() - 1)];
}
