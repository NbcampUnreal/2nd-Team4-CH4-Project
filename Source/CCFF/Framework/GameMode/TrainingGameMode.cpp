#include "Framework/GameMode/TrainingGameMode.h"
#include "Framework/GameState/TrainingGameState.h"
#include "Character/Base/BaseCharacter.h"
#include "Framework/Controller/TrainingPlayerController.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


ATrainingGameMode::ATrainingGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PlayerControllerClass = ATrainingPlayerController::StaticClass();
    GameStateClass = ATrainingGameState::StaticClass();
}

void ATrainingGameMode::BeginPlay()
{
    Super::BeginPlay();

    RegisterTrainingBotDamageEvents();
}

void ATrainingGameMode::RegisterTrainingBotDamageEvents()
{
    TArray<AActor*> TrainingBots;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), TrainingBots);

    for (AActor* Bot : TrainingBots)
    {
        if (ABaseCharacter* TrainingBot = Cast<ABaseCharacter>(Bot))
        {
            TrainingBot->OnTakeAnyDamage.AddDynamic(this, &ATrainingGameMode::HandleBotDamage);
        }
    }
}

void ATrainingGameMode::HandleBotDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (ATrainingPlayerController* TrainingPlayerController = Cast<ATrainingPlayerController>(InstigatedBy))
    {
        TrainingPlayerController->ClientAddLocalDamage(Damage);
    }
}



