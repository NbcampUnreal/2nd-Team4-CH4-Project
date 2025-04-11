#include "Framework/GameState/TrainingGameState.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"

ATrainingGameState::ATrainingGameState()
	: TrainingRemainingTime(0.f),
	TotalDamage(0.f),
	DPS(0.f)
{
}

void ATrainingGameState::InitializeGameState()
{
	Super::InitializeGameState();

	TrainingRemainingTime = 0.0f;
	TotalDamage = 0.0f;
	DPS = 0.0f;
}

//void ATrainingGameState::SetRoundProgress(ERoundProgress NewProgress)
//{
//	if (HasAuthority())
//	{
//		TrainingRoundProcess = NewProgress;
//		OnRep_TrainingRoundProcess();
//	}
//}

//void ATrainingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ATrainingGameState, TrainingRoundProcess);
//	DOREPLIFETIME(ATrainingGameState, TrainingRemainingTime);
//	DOREPLIFETIME(ATrainingGameState, TotalDamage);
//	DOREPLIFETIME(ATrainingGameState, DPS);
//}
//
//void ATrainingGameState::OnRep_TrainingRoundProcess()
//{
//	UE_LOG(LogTemp, Log, TEXT("[TrainingGameState] TrainingRoundProgress changed to: %d"), static_cast<uint8>(TrainingRoundProcess));
//}
//
//void ATrainingGameState::OnRep_TrainingRemainingTime()
//{
//	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
//	{
//		if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(PC->GetHUD()))
//		{
//			if (UTrainingWidget* W = HUD->GetTrainingWidget())
//			{
//				W->UpdateTimer(TrainingRemainingTime);
//			}
//		}
//	}
//}
//
//void ATrainingGameState::OnRep_TotalDamage()
//{
//	OnRep_DPS();
//}
//
//void ATrainingGameState::OnRep_DPS()
//{
//	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
//	{
//		if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(PC->GetHUD()))
//		{
//			if (UTrainingWidget* W = HUD->GetTrainingWidget())
//			{
//				// TotalDamage와 DPS 값을 위젯에 전달
//				W->UpdateTrainingStatsData(TotalDamage, DPS);
//			}
//		}
//	}
//}
