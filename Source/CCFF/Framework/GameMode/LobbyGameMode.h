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

	virtual void PreLogin(
		const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage
	) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void NotifyPlayerReadyStatusChanged();

	void StartGameWithDelay();
	void StartGame();

#pragma region FORBID_SOLO_PLAYING
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test")
	bool bAllowSoloStart = false;

#pragma endregion

	UPROPERTY()
	TArray<AActor*> PlayerSpawnSlots;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	TSubclassOf<class ALobbyPreviewPawn> PreviewPawnClass;
	
	TMap<APlayerController*, int32> AssignedSlotIndices;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	TArray<FString> AvailableMapPaths;

	FTimerHandle GameStartTimerHandle;

};
