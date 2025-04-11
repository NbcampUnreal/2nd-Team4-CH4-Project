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

protected:
    UFUNCTION()
    void HandleBotDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    void RegisterTrainingBotDamageEvents();
};
