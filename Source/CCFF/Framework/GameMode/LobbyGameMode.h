#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class CCFF_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void NotifyPlayerReadyStatusChanged();

	void StartGameWithDelay();
	void StartGame();

	UPROPERTY()
	TArray<AActor*> PlayerSpawnSlots;

	TMap<APlayerController*, int32> AssignedSlotIndices;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	TArray<FString> AvailableMapPaths;

	FTimerHandle GameStartTimerHandle;

};
