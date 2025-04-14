#pragma once

#include "CoreMinimal.h"
#include "Framework/Controller/CharacterController.h"
#include "TrainingPlayerController.generated.h"


UCLASS()
class CCFF_API ATrainingPlayerController : public ACharacterController
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
    void StartLocalTraining(float InTime);
    void AddLocalDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
    void EndLocalTraining();

    UFUNCTION(Client, Reliable)
    void ClientAddLocalDamage(float Damage);
	
protected:
    void UpdateLocalTraining();

    FTimerHandle LocalTrainingTimerHandle;

    float LocalInitialTime;
    float LocalRemainingTime;
    float LocalTotalDamage;
    float LocalDPS;
};
