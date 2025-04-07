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

	UFUNCTION(BlueprintCallable, Category = "Training Stats")
	void AddDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Training Stats")
	void UpdateMaxCombo(int32 CurrentCombo);

	UFUNCTION(BlueprintCallable, Category = "Training Stats")
	void CalculateDPS(float TrainingTime);

	UFUNCTION(BlueprintCallable, Category = "Training Stats")
	void UpdateTrainingStats(float TrainingTime);

	UPROPERTY(BlueprintReadOnly, Category = "Training Stats")
	float TotalDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Training Stats")
	int32 MaxCombo;

	UPROPERTY(BlueprintReadOnly, Category = "Training Stats")
	float DPS;
};
