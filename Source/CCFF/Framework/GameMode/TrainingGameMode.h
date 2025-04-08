#pragma once

#include "CoreMinimal.h"
#include "Framework/GameMode/BaseInGameMode.h"
#include "TrainingGameMode.generated.h"


UCLASS()
class CCFF_API ATrainingGameMode : public ABaseInGameMode
{
	GENERATED_BODY()

public:
	ATrainingGameMode(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetRoundTime(float InTime);

	void StartTraining();
	virtual void EndRound() override;
	virtual void CheckGameConditions() override;

#pragma region TrainingData
	UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
	void AddDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
	void UpdateTrainingStats();

	UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
	void RegisterTrainingBotDamageEvents();

	UFUNCTION()
	void HandleBotDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
#pragma endregion	

protected:
	UPROPERTY()
	FTimerHandle TrainingStatusTimerHandle;
};
