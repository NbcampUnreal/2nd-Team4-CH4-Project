#include "Framework/GameMode/BaseInGameMode.h"
#include "Framework/GameState/BaseInGameState.h"
#include "Items/Manager/ItemManager.h"

ABaseInGameMode::ABaseInGameMode(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = ABaseInGameState::StaticClass();

	RoundDuration = 60.0f;
	MyClassName = "BaseGameMode";
}

void ABaseInGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartRound();
	ResetSubsystem();
}

void ABaseInGameMode::ResetSubsystem()
{
	UItemManager* ItemManager = GetGameInstance()->GetSubsystem<UItemManager>();
	if (ItemManager)
	{
		ItemManager->InitializeItemManager();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemManager is not initialized!"));
	}
}

void ABaseInGameMode::StartRound()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Start Round!"), *MyClassName);

	// CheckCondition every second
	GetWorldTimerManager().SetTimer(
		GameTimerHandle, this, &ABaseInGameMode::CheckGameConditions, 1.0f, true
		);
}

void ABaseInGameMode::EndRound()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] End Round!"), *MyClassName);

	GetWorldTimerManager().ClearTimer(GameTimerHandle);
}

void ABaseInGameMode::CheckGameConditions()
{
	// Call EndRound() when condition is true
}
