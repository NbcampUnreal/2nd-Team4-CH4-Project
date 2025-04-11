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

#pragma region Arena
	// TODO :: 업적 시스템(어시스트, 등등)

public:
	void UpdateArenaStats();
	void UpdatePlayerRating();
	void UpdateCountdown();

	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	float CountdownTime;

#pragma endregion

	/* 게임 시작 이후 입장 제한 */
	UPROPERTY()
	bool bHasGameStarted = false;

private:
	UFUNCTION()
	void MoveResultLevel();

	FTimerHandle ArenaTimerHandle;
	FTimerHandle CountdownTimerHandle;
	FTimerHandle LevelTransitionTimerHandle;

	bool bIsStartedRound;

};
