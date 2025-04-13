#pragma once

#include "CoreMinimal.h"
#include "Framework/GameState/BaseInGameState.h"
#include "ArenaGameState.generated.h"

USTRUCT(BlueprintType)
struct FArenaRankInfo
{
	GENERATED_BODY()

	FArenaRankInfo()
		: Rank(0)
		, PlayerName(TEXT(""))
		, TotalDamage(0.f)
		, SurvivalTime(0.f)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ranking")
	int32 Rank;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ranking")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ranking")
	float TotalDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ranking")
	float SurvivalTime;
};

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

	FORCEINLINE float GetRoundStartTime() { return RoundStartTime; }
	FORCEINLINE void SetRoundStartTime(float Time) { RoundStartTime = Time; }

	UFUNCTION(BlueprintCallable, Category = "Ranking")
	void SetRankingInfos(const TArray<FArenaRankInfo>& Infos) { RankingInfos = Infos; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ranking")
	TArray<FArenaRankInfo> RankingInfos;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Arena|State")
	float RoundStartTime;

};
