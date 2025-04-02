#include "Framework/GameState/BaseInGameState.h"
#include "Net/UnrealNetwork.h"

ABaseInGameState::ABaseInGameState()
{
	RemainingTime = 0.0f;
	RoundProgress = ERoundProgress::NotStarted;
}

void ABaseInGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, RoundProgress);
}

void ABaseInGameState::UpdateHUDData()
{
	UE_LOG(LogTemp, Log, TEXT("HUD Updated: RemainingTime: %f, RoundProgress: %d"), RemainingTime, (uint8)RoundProgress);
}

void ABaseInGameState::SetRemainingTime(float NewTime)
{
	RemainingTime = NewTime;
	UpdateHUDData();
}

void ABaseInGameState::SetRoundProgress(ERoundProgress NewProgress)
{
	RoundProgress = NewProgress;
	UpdateHUDData();
}
