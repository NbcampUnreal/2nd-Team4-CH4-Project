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

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		SpawnPlayer(PC);
	}

    RegisterTrainingBotDamageEvents();
}

void ATrainingGameMode::SpawnPlayer(AController* NewPlayer)
{
	if (!NewPlayer) return;

	FName SelectedID = NAME_None;
	SelectedID = "Cactus";


	if (!CharacterClasses.Contains(SelectedID))
	{
		UE_LOG(LogTemp, Warning, TEXT("++++++++++++++++++[SpawnPlayer] '%s' no mapping"), *SelectedID.ToString());
		return;
	}
	TSubclassOf<APawn> PawnClass = CharacterClasses[SelectedID];

	AActor* StartSpot = ChoosePlayerStart(NewPlayer);
	const FTransform StartTransform = StartSpot ? StartSpot->GetActorTransform() : FTransform::Identity;
	const FVector SpawnLocation = StartTransform.GetLocation();
	const float SpawnYaw = StartTransform.GetRotation().Rotator().Yaw;

	const FRotator SpawnRotation(0.f, SpawnYaw, 0.f);

	FActorSpawnParameters Params;
	Params.Owner = NewPlayer;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnLocation, SpawnRotation, Params);
	if (NewPawn)
	{
		NewPlayer->Possess(NewPawn);

		if (APlayerController* PC = Cast<APlayerController>(NewPlayer))
		{
			FRotator CR = PC->GetControlRotation();
			PC->SetControlRotation(CR);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("++++++++++++++++++[SpawnPlayer] Spawn Fail : %s"), *PawnClass->GetName());
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



