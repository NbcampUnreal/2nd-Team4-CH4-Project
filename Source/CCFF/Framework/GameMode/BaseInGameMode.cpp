#include "Framework/GameMode/BaseInGameMode.h"
#include "Framework/GameState/BaseInGameState.h"

ABaseInGameMode::ABaseInGameMode(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = ABaseInGameState::StaticClass();

	RoundDuration = 60.0f;
	MaxHP = 3;
}

void ABaseInGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartRound();
}

void ABaseInGameMode::StartRound()
{
	UE_LOG(LogTemp, Log, TEXT("Start Round!"));

	GetWorldTimerManager().SetTimer(
		GameTimerHandle, this, &ABaseInGameMode::CheckGameConditions, 1.0f, true
		);
}

void ABaseInGameMode::EndRound()
{
	UE_LOG(LogTemp, Log, TEXT("End Round!"));

	GetWorldTimerManager().ClearTimer(GameTimerHandle);
}

void ABaseInGameMode::CheckGameConditions()
{
}
