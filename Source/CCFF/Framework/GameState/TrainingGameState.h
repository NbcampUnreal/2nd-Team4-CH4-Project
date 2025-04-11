#pragma once

#include "CoreMinimal.h"
#include "Framework/GameState/BaseInGameState.h"
#include "TrainingGameState.generated.h"


UCLASS()
class CCFF_API ATrainingGameState : public ABaseInGameState
{
	GENERATED_BODY()

public:
	ATrainingGameState();

	void InitializeGameState() override;

	FORCEINLINE ERoundProgress GetRoundProgress() const { return TrainingRoundProcess; }
	FORCEINLINE void SetRoundProgress(ERoundProgress NewProgress) { TrainingRoundProcess = NewProgress; }
	
	FORCEINLINE float GetTotalDamage() const { return TotalDamage; }
	FORCEINLINE void SetTotalDamage(float Damage) { TotalDamage = Damage; }

	FORCEINLINE float GetDPS() const { return DPS; }
	FORCEINLINE void SetDPS(float InDPS) { DPS = InDPS; }

	FORCEINLINE float GetRemainingTime() const { return TrainingRemainingTime; }
	FORCEINLINE void SetRemainingTime(float Time) { TrainingRemainingTime = Time; }

#pragma region DPS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CCFF|Training Stats")
	ERoundProgress TrainingRoundProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CCFF|Training Stats")
	float TrainingRemainingTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CCFF|Training Stats")
	float TotalDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CCFF|Training Stats")
	float DPS;
#pragma endregion
};
