#pragma once

#include "CoreMinimal.h"
#include "Framework/GameMode/BaseInGameMode.h"
#include "Character/Base/BaseCharacter.h"
#include "TrainingGameMode.generated.h"


UCLASS()
class CCFF_API ATrainingGameMode : public ABaseInGameMode
{
	GENERATED_BODY()

public:
    ATrainingGameMode();

    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void BeginPlay() override;

    UFUNCTION()
    void SpawnPlayer(APlayerController* NewPlayer);

    UPROPERTY(EditDefaultsOnly, Category = "Character")
    TMap<FName, TSubclassOf<ABaseCharacter>> CharacterClasses;

protected:
    UFUNCTION()
    void HandleBotDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    void RegisterTrainingBotDamageEvents();
};
