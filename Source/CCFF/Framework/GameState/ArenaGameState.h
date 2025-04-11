#pragma once

#include "CoreMinimal.h"
#include "Framework/GameState/BaseInGameState.h"
#include "ArenaGameState.generated.h"


UCLASS()
class CCFF_API AArenaGameState : public ABaseInGameState
{
	GENERATED_BODY()
	
public:
	AArenaGameState();

	virtual void InitializeGameState() override;

	FORCEINLINE ERoundProgress GetRoundProgress() const { return ArenaRoundProgress; }
	void SetRoundProgress(ERoundProgress NewProgress);

	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
	FORCEINLINE void SetCountdownTime(float Time) { CountdownTime = Time; }

	FORCEINLINE float GetRemainingTime() const { return ArenaRemainingTime; }
	FORCEINLINE void SetRemainingTime(float Time) { ArenaRemainingTime = Time; }
	
	UFUNCTION()
	void OnRep_ArenaRoundProgress();
	UFUNCTION()
	void OnRep_CountdownTime();
	UFUNCTION()
	void OnRep_FinishCountdown();
	UFUNCTION()
	void OnRep_ArenaRemainingTime();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ArenaRoundProgress, Category = "Arena|State")
	ERoundProgress ArenaRoundProgress;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CountdownTime, Category = "Arena|State")
	float CountdownTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_FinishCountdown, Category = "Arena|State")
	bool bIsFinishCountdown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ArenaRemainingTime, Category = "Arena|State")
	float ArenaRemainingTime;

};
