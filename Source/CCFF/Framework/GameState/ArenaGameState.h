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

	FORCEINLINE void SetRoundProgress(ERoundProgress NewProgress) { RoundProgress = NewProgress; }
	FORCEINLINE ERoundProgress GetRoundProgress() { return RoundProgress; }

	FORCEINLINE float GetCountdownTime() { return CountdownTime; }
	FORCEINLINE void SetCountdownTime(float Time) { CountdownTime = Time; }
	
	FORCEINLINE float GetTotalDamage() const { return TotalDamage; }
	FORCEINLINE void SetTotalDamage(float Damage) { TotalDamage = Damage; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Arena|State")
	float CountdownTime;

	float TotalDamage;
};
