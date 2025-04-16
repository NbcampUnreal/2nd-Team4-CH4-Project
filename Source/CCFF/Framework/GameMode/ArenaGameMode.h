#pragma once

#include "CoreMinimal.h"
#include "Framework/GameMode/BaseInGameMode.h"
#include <Framework/Data/ArenaSubModeType.h>
#include "ArenaGameMode.generated.h"


class ABaseCharacter;

UCLASS()
class CCFF_API AArenaGameMode : public ABaseInGameMode
{
	GENERATED_BODY()
	
public:
	AArenaGameMode();

	virtual void BeginPlay() override;
	virtual void PreLogin(
		const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage)
	override;

	void StartArenaRound();
	virtual void EndRound() override;
	virtual void CheckGameConditions() override;

	virtual void ResetSubsystem() override;

	FORCEINLINE ACameraActor* GetSpectatorCamera() const { return SpectatorCamera; }

#pragma region Arena

public:
	void UpdateArenaStats();
	void UpdatePlayerRating();
	void UpdateCountdown();

	UFUNCTION()
	void RespawnPlayer(AController* Controller);
	
	EArenaSubMode SelectedArenaSubMode;

	UPROPERTY(EditDefaultsOnly, Category = "CCFF|Arena")
	float CountdownTime;

#pragma endregion

	UPROPERTY()
	bool bHasGameStarted = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CCFF|Ranking")
	float DamageWeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CCFF|Ranking")
	float TimeWeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CCFF|Ranking")
	float KillCountWeight;

	UPROPERTY()
	ACameraActor* SpectatorCamera = nullptr;


private:
	FTimerHandle ArenaTimerHandle;
	FTimerHandle CountdownTimerHandle;
	FTimerHandle LevelTransitionTimerHandle;
	FTimerHandle ConditionCheckTimerHandle;

	bool bIsStartedRound;

#pragma region SpawnCharacter
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TMap<FName, TSubclassOf<APawn>> CharacterClasses;

	UFUNCTION()
	void SpawnPlayer(AController* NewPlayer);
};
