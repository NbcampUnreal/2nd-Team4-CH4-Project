#include "Framework/GameState/TrainingGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
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
	UE_LOG(LogTemp, Log, TEXT("Total Damage updated: %f"), TotalDamage);
}

void ATrainingGameState::UpdateMaxCombo(int32 CurrentCombo)
{
	if (CurrentCombo > MaxCombo)
	{
		MaxCombo = CurrentCombo;
		UE_LOG(LogTemp, Log, TEXT("Max Combo updated: %d"), MaxCombo);
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

void ATrainingGameState::UpdateTrainingStats(float TrainingTime)
{
	CalculateDPS(TrainingTime);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(PC->GetHUD()))
		{
			if (UTrainingWidget* TrainingWidget = HUD->GetTrainingWidget())
			{
				TrainingWidget->UpdateTrainingStatsData(TotalDamage, MaxCombo, DPS);
			}
		}
	}
}