#pragma once

#include "CoreMinimal.h"
#include "Framework/GameMode/BaseInGameMode.h"
#include "TrainingGameMode.generated.h"


UCLASS()
class CCFF_API ATrainingGameMode : public ABaseInGameMode
{
	GENERATED_BODY()

public:
    ATrainingGameMode();

    virtual void BeginPlay() override;

    UFUNCTION()
    void SpawnPlayer(AController* NewPlayer);

    UPROPERTY(EditDefaultsOnly, Category = "Character")
    TMap<FName, TSubclassOf<APawn>> CharacterClasses;

protected:
    UFUNCTION()
    void HandleBotDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    void RegisterTrainingBotDamageEvents();
};
