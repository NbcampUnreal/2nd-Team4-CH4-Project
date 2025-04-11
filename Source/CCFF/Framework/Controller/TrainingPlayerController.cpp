#include "Framework/Controller/TrainingPlayerController.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"

void ATrainingPlayerController::StartLocalTraining(float InTime)
{
    // �ʱⰪ ����
    LocalRemainingTime = InTime;
    LocalTotalDamage = 0.f;
    LocalDPS = 0.f;

    // 1�ʸ��� UpdateLocalTraining() ȣ��
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
    // ���� �ð� ����
    if (LocalRemainingTime <= 1.f)
    {
        EndLocalTraining();
        return;
    }
    LocalRemainingTime -= 1.f;

    // DPS ���
    float Elapsed = (/*�ʱ� InTime*/ LocalRemainingTime + 1.f) - LocalRemainingTime;
    LocalDPS = (Elapsed > 0.f) ? (LocalTotalDamage / Elapsed) : 0.f;

    // UI ����
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
    // �ʿ� �� ��� UI ����
    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
        if (UTrainingWidget* W = HUD->GetTrainingWidget())
        {
            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
}

void ATrainingPlayerController::EndLocalTraining()
{
    GetWorld()->GetTimerManager().ClearTimer(LocalTrainingTimerHandle);

    // ���� �� UI�� �ݿ�
    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
        if (UTrainingWidget* W = HUD->GetTrainingWidget())
        {
            W->UpdateTimer(0.f);
            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
}