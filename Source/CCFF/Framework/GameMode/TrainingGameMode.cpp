#include "Framework/GameMode/TrainingGameMode.h"
#include "Framework/GameState/TrainingGameState.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "Character/Base/BaseCharacter.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Controller/TrainingPlayerController.h"


ATrainingGameMode::ATrainingGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 전용 PlayerController 지정
    PlayerControllerClass = ATrainingPlayerController::StaticClass();
    HUDClass = ATrainingModeHUD::StaticClass();
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
            TrainingBot->OnTakeAnyDamage.AddDynamic(
                this,
                &ATrainingGameMode::HandleBotDamage
            );
        }
    }
}

void ATrainingGameMode::HandleBotDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (ATrainingPlayerController* TPC = Cast<ATrainingPlayerController>(InstigatedBy))
    {
        TPC->ClientAddLocalDamage(Damage);
        UE_LOG(LogTemp, Log, TEXT("HandleBotDamage: Called ClientAddLocalDamage on %s"), *TPC->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HandleBotDamage: InstigatedBy is not TrainingPlayerController"));
    }
}



