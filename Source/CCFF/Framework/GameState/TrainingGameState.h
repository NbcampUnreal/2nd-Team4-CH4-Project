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

	UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
	void AddDamage(float DamageAmount);
	UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
	void UpdateMaxCombo(int32 CurrentCombo);
	UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
	void CalculateDPS(float TrainingTime);

	UPROPERTY(VisibleAnywhere, Category = "CCFF|Training")
	float TotalDamage;
	UPROPERTY(VisibleAnywhere, Category = "CCFF|Training")
	int32 MaxCombo;
	UPROPERTY(VisibleAnywhere, Category = "CCFF|Training")
	float DPS;
};
