#include "Framework/GameState/TrainingGameState.h"
#include "Net/UnrealNetwork.h"

ATrainingGameState::ATrainingGameState()
{
	TotalDamage = 0.0f;
	MaxCombo = 0;
	DPS = 0.0f;
}

void ATrainingGameState::AddDamage(float DamageAmount)
{
	TotalDamage += DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("Total Damage: %f"), TotalDamage);
}

void ATrainingGameState::UpdateMaxCombo(int32 CurrentCombo)
{
	if (CurrentCombo > MaxCombo)
	{
		MaxCombo = CurrentCombo;
		UE_LOG(LogTemp, Log, TEXT("Max Combo : %d"), MaxCombo);
	}
}

void ATrainingGameState::CalculateDPS(float TrainingTime)
{
	if (TrainingTime > 0.0f)
	{
		DPS = TotalDamage / TrainingTime;
	}
	else
	{
		DPS = 0.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("Calculated DPS: %f"), DPS);
}
