#include "Framework/GameMode/TrainingGameMode.h"
#include "Framework/GameState/TrainingGameState.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Character/Base/BaseCharacter.h"
#include "Framework/Controller/TrainingPlayerController.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


ATrainingGameMode::ATrainingGameMode()
{
    PlayerControllerClass = ATrainingPlayerController::StaticClass();
    GameStateClass = ATrainingGameState::StaticClass();
}

void ATrainingGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (NewPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("+++++++++++++++++++++++++   SpawnPlayer"));
        SpawnPlayer(NewPlayer);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("+++++++++++++++++++++++++   No PlayerContoller"));
    }
}

void ATrainingGameMode::BeginPlay()
{
    Super::BeginPlay();

	if (UCCFFGameInstance* CCFFGameInstance = GetGameInstance<UCCFFGameInstance>())
	{
		CCFFGameInstance->PlayBGMForCurrentMap();
	}

    RegisterTrainingBotDamageEvents();
}

void ATrainingGameMode::SpawnPlayer(APlayerController* NewPlayer)
{
    if (!NewPlayer) return;

    const FName SelectedID = "Cactus";
    if (!CharacterClasses.Contains(SelectedID))
    {
        UE_LOG(LogTemp, Warning, TEXT("+++++++++++++++++++++++ [SpawnPlayer] '%s' no mapping"), *SelectedID.ToString());
        return;
    }

    TSubclassOf<ABaseCharacter> CharacterClass = CharacterClasses[SelectedID];

    AActor* StartSpot = ChoosePlayerStart(NewPlayer);
    const FTransform StartTransform = StartSpot
        ? StartSpot->GetActorTransform()
        : FTransform::Identity;

    FActorSpawnParameters Params;
    Params.Owner = NewPlayer;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    ABaseCharacter* NewCharacter = GetWorld()->SpawnActor<ABaseCharacter>(
        CharacterClass,
        StartTransform.GetLocation(),
        StartTransform.GetRotation().Rotator(),
        Params
    );

    if (!NewCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("+++++++++++++++++++++++ [SpawnPlayer] Spawn failed: %s"), *CharacterClass->GetName());
        return;
    }

    // Possess
    NewPlayer->Possess(NewCharacter);
    if (ATrainingPlayerController* PC = Cast<ATrainingPlayerController>(NewPlayer))
    {
        PC->SetControlRotation(PC->GetControlRotation());
    }
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



