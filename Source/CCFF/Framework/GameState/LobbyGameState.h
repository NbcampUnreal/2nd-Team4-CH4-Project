#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Framework/Data/ArenaSubModeType.h"
#include "LobbyGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllowStartGameChanged, bool);

UCLASS()
class CCFF_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
#pragma region GAMESTART
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	bool EvaluateStartCondition() const;

	UFUNCTION()
	int32 GetPlayerCount() const;

	UFUNCTION()
	int32 GetReadyPlayerCount() const;

	void UpdateAllowStartGame();

	UFUNCTION()
	void OnRep_bAllowStartGame();

	FOnAllowStartGameChanged OnAllowStartGameChanged;

	UPROPERTY(ReplicatedUsing = OnRep_bAllowStartGame, BlueprintReadOnly, Category = "Lobby")
	bool bAllowStartGame = false;
#pragma endregion

#pragma region ArenaSubMode
public:
	UFUNCTION()
	void SetArenaSubMode(EArenaSubMode NewMode);

	UFUNCTION()
	void OnRep_ArenaSubMode();

	UPROPERTY(ReplicatedUsing = OnRep_ArenaSubMode, BlueprintReadOnly)
	EArenaSubMode ArenaSubMode;
#pragma endregion

public:
#pragma region COUNTDOWN
	UFUNCTION()
	void OnRep_RemainingCountdownTime();

	UFUNCTION()
	void ResetCountdown();

	void StartCountdownTimer();
	void TickCountdownTimer();

	UPROPERTY(ReplicatedUsing = OnRep_RemainingCountdownTime)
	int32 RemainingCountdownTime = -1;

	FTimerHandle CountdownTickHandle;
#pragma endregion

};
