#include "Framework/Controller/TrainingPlayerController.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "Framework/GameState/TrainingGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

void ATrainingPlayerController::StartLocalTraining(float InTime)
{
   LocalInitialTime = InTime;
   LocalRemainingTime = InTime;
   LocalTotalDamage = 0.f;
   LocalDPS = 0.f;

   if (ATrainingGameState* TGameState = Cast<ATrainingGameState>(GetWorld()->GetGameState<ATrainingGameState>()))
   {
       TGameState->SetRoundProgress(ERoundProgress::InProgress);
   }

   GetWorld()->GetTimerManager().SetTimer(
       LocalTrainingTimerHandle,
       this,
       &ATrainingPlayerController::UpdateLocalTraining,
       1.0f,
       true
   );
}

void ATrainingPlayerController::UpdateLocalTraining()
{
    if (LocalRemainingTime <= 0.f)
    {
        EndLocalTraining();
        return;
    }

    LocalRemainingTime -= 1.f;

    float Elapsed = LocalInitialTime - LocalRemainingTime;
    LocalDPS = (Elapsed > 0.f) ? (LocalTotalDamage / Elapsed) : 0.f;

    if (ATrainingModeHUD* TrainingModeHUD = Cast<ATrainingModeHUD>(GetHUD()))
    {
        if (UTrainingWidget* TrainingWidget = TrainingModeHUD->GetTrainingWidget())
        {
            TrainingWidget->UpdateTimer(LocalRemainingTime);
            TrainingWidget->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
    }
}

void ATrainingPlayerController::ClientAddLocalDamage_Implementation(float Damage)
{
    ATrainingGameState* TrainingGameState = Cast<ATrainingGameState>(GetWorld()->GetGameState());
    if (IsValid(TrainingGameState))
    {
        if (TrainingGameState->GetRoundProgress() == ERoundProgress::InProgress)
            AddLocalDamage(Damage);
    }
}

void ATrainingPlayerController::AddLocalDamage(float DamageAmount)
{
    LocalTotalDamage += DamageAmount;
    if (IsLocalController())
    {
        if (ATrainingModeHUD* TrainingModeHUD = Cast<ATrainingModeHUD>(GetHUD()))
        {
            if (UTrainingWidget* TrainingWidget = TrainingModeHUD->GetTrainingWidget())
            {
                TrainingWidget->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
            }
        }
    }
}

void ATrainingPlayerController::EndLocalTraining()
{
    GetWorld()->GetTimerManager().ClearTimer(LocalTrainingTimerHandle);

    ATrainingGameState* TrainingGameState = Cast<ATrainingGameState>(GetWorld()->GetGameState());
    if (IsValid(TrainingGameState))
    {
        if (ATrainingModeHUD* TrainingModeHUD = Cast<ATrainingModeHUD>(GetHUD()))
        {
            if (UTrainingWidget* TrainingWidget = TrainingModeHUD->GetTrainingWidget())
            {
                TrainingWidget->UpdateTimer(0.f);
                TrainingWidget->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
                TrainingGameState->SetRoundProgress(ERoundProgress::Ended);
            }
        }
    }
}