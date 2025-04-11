#include "Framework/Controller/TrainingPlayerController.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"

void ATrainingPlayerController::StartLocalTraining(float InTime)
{
    // 초기값 세팅
    LocalRemainingTime = InTime;
    LocalTotalDamage = 0.f;
    LocalDPS = 0.f;

    // 1초마다 UpdateLocalTraining() 호출
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
    // 남은 시간 감소
    if (LocalRemainingTime <= 1.f)
    {
        EndLocalTraining();
        return;
    }
    LocalRemainingTime -= 1.f;

    // DPS 계산
    float Elapsed = (/*초기 InTime*/ LocalRemainingTime + 1.f) - LocalRemainingTime;
    LocalDPS = (Elapsed > 0.f) ? (LocalTotalDamage / Elapsed) : 0.f;

    // UI 갱신
    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
        if (UTrainingWidget* W = HUD->GetTrainingWidget())
        {
            W->UpdateTimer(LocalRemainingTime);
            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
}

void ATrainingPlayerController::AddLocalDamage(float DamageAmount)
{
    LocalTotalDamage += DamageAmount;
    // 필요 시 즉시 UI 갱신
    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
        if (UTrainingWidget* W = HUD->GetTrainingWidget())
        {
            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
}

void ATrainingPlayerController::EndLocalTraining()
{
    GetWorld()->GetTimerManager().ClearTimer(LocalTrainingTimerHandle);

    // 최종 값 UI에 반영
    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
        if (UTrainingWidget* W = HUD->GetTrainingWidget())
        {
            W->UpdateTimer(0.f);
            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
}