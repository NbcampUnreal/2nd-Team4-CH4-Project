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

	FORCEINLINE void SetRoundProgress(ERoundProgress NewProgress) { TrainingRoundProcess = NewProgress; }
	FORCEINLINE ERoundProgress GetRoundProgress() { return TrainingRoundProcess; }
	FORCEINLINE float GetTotalDamage() const { return TotalDamage; }
	FORCEINLINE void SetTotalDamage(float Damage) { TotalDamage = Damage; }
	FORCEINLINE float GetDPS() const { return DPS; }
	FORCEINLINE void SetDPS(float InDPS) { DPS = InDPS; }
	FORCEINLINE float GetRemainingTime() const { return TrainingRemainingTime; }
	FORCEINLINE void SetRemainingTime(float Time) { TrainingRemainingTime = Time; }

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION()
	//void OnRep_TrainingRoundProcess();

	//UFUNCTION()
	//void OnRep_TrainingRemainingTime();

	//UFUNCTION()
	//void OnRep_TotalDamage();

	//UFUNCTION()
	//void OnRep_DPS();

#pragma region DPS
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_TrainingRoundProcess, Category = "CCFF|Training Stats")
	ERoundProgress TrainingRoundProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_TrainingRemainingTime, Category = "CCFF|Training Stats")
	float TrainingRemainingTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_TotalDamage, Category = "CCFF|Training Stats")
	float TotalDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_DPS, Category = "CCFF|Training Stats")
	float DPS;*/
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
