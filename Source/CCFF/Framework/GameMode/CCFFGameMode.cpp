#include "Framework/GameMode/CCFFGameMode.h"
#include "Framework/GameState/CCFFGameState.h"

ACCFFGameMode::ACCFFGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = ACCFFGameState::StaticClass();

	RoundDuration = 3.0f * 60.0f;  // 일단 3분으로 설정
	MaxHP = 3;
}

void ACCFFGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartRound();
}

void ACCFFGameMode::StartRound()
{
	UE_LOG(LogTemp, Log, TEXT("[CCFFGameMode] Start Round!"));

	GetWorldTimerManager().SetTimer(
		GameTimerHandle, this, &ACCFFGameMode::CheckGameConditions, 1.0f, true
		);

}

void ACCFFGameMode::EndRound()
{
	UE_LOG(LogTemp, Log, TEXT("[CCFFGameMode] End Round!"));

	GetWorldTimerManager().ClearTimer(GameTimerHandle);
}

void ACCFFGameMode::CheckGameConditions()
{
}
