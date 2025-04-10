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

	FORCEINLINE void SetRoundProgress(ERoundProgress NewProgress) { RoundProgress = NewProgress; }
	FORCEINLINE ERoundProgress GetRoundProgress() { return RoundProgress; }
	FORCEINLINE float GetTotalDamage() const { return TotalDamage; }
	FORCEINLINE void SetTotalDamage(float Damage) { TotalDamage = Damage; }
	FORCEINLINE float GetDPS() const { return DPS; }
	FORCEINLINE void SetDPS(float InDPS) { DPS = InDPS; }

#pragma region DPS
	float RemainingTime;

	UPROPERTY(BlueprintReadOnly, Category = "CCFF|Training Stats")
	float TotalDamage;

	UPROPERTY(BlueprintReadOnly, Category = "CCFF|Training Stats")
	float DPS;
#pragma endregion
};
