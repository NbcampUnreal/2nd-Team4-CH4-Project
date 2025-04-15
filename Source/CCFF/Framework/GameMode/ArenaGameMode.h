#pragma once

#include "CoreMinimal.h"
#include "Framework/GameMode/BaseInGameMode.h"
#include "ArenaGameMode.generated.h"

class ABaseCharacter;

UCLASS()
class CCFF_API AArenaGameMode : public ABaseInGameMode
{
	GENERATED_BODY()
	
public:
	AArenaGameMode(const FObjectInitializer& ObjectInitializer);

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

	FORCEINLINE ACameraActor* GetSpectatorCamera() const { return SpectatorCamera; }

#pragma region Arena
	// TODO :: 업적 시스템(어시스트, 등등)

public:
	void UpdateArenaStats();
	void UpdatePlayerRating();
	void UpdateCountdown();

	UFUNCTION()
	void RespawnPlayer(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "CCFF|Arena")
	float CountdownTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Rules")
	bool bIsDeathmatch = false;

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

	bool bIsStartedRound;

};
