#include "Framework/Controller/TrainingPlayerController.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "Framework/GameState/TrainingGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "GameFramework/GameStateBase.h" // �߰��� include
#include <Kismet/GameplayStatics.h>

void ATrainingPlayerController::StartLocalTraining(float InTime)
{
    UE_LOG(LogTemp, Log, TEXT("StartLocalTraining: InTime = %.2f"), InTime);

   // �ʱⰪ ����
   LocalInitialTime = InTime;
   LocalRemainingTime = InTime;
   LocalTotalDamage = 0.f;
   LocalDPS = 0.f;

   if (ATrainingGameState* TGameState = Cast<ATrainingGameState>(GetWorld()->GetGameState<ATrainingGameState>()))
   {
       TGameState->SetRoundProgress(ERoundProgress::InProgress);
   }

   // 1�ʸ��� UpdateLocalTraining() ȣ��
   GetWorld()->GetTimerManager().SetTimer(
       LocalTrainingTimerHandle,
       this,
       &ATrainingPlayerController::UpdateLocalTraining,
       1.0f,
       true
   );
}

//void ATrainingPlayerController::UpdateLocalTraining()
//{
//    // ���� �ð� ����
//    if (LocalRemainingTime <= 0.f)
//    {
//        EndLocalTraining();
//        return;
//    }
//    LocalRemainingTime -= 1.f;
//
//    // DPS ���
//    float Elapsed = LocalInitialTime - LocalRemainingTime;
//    LocalDPS = (Elapsed > 0.f) ? (LocalTotalDamage / Elapsed) : 0.f;
//
//    // UI ����
//    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
//    {
//        if (UTrainingWidget* W = HUD->GetTrainingWidget())
//        {
//            W->UpdateTimer(LocalRemainingTime);
//            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
//            UE_LOG(LogTemp, Log, TEXT("%.2f, %.2f"), LocalTotalDamage, LocalDPS);
//        }
//    }
//}
void ATrainingPlayerController::UpdateLocalTraining()
{
    UE_LOG(LogTemp, Log, TEXT("UpdateLocalTraining: Before update: RemainingTime = %.2f, TotalDamage = %.2f, DPS = %.2f"),
        LocalRemainingTime, LocalTotalDamage, LocalDPS);

    if (LocalRemainingTime <= 0.f)
    {
        EndLocalTraining();
        return;
    }
    LocalRemainingTime -= 1.f;

    float Elapsed = LocalInitialTime - LocalRemainingTime;
    UE_LOG(LogTemp, Log, TEXT("Elapsed %.2f"), Elapsed);

    LocalDPS = (Elapsed > 0.f) ? (LocalTotalDamage / Elapsed) : 0.f;

    UE_LOG(LogTemp, Log, TEXT("UpdateLocalTraining: After update: RemainingTime = %.2f, TotalDamage = %.2f, DPS = %.2f"),
        LocalRemainingTime, LocalTotalDamage, LocalDPS);

    // UI ����
    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
    {
        if (UTrainingWidget* W = HUD->GetTrainingWidget())
        {
            W->UpdateTimer(LocalRemainingTime);
            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
    }
}


void ATrainingPlayerController::ClientAddLocalDamage_Implementation(float Damage)
{
    // Ŭ���̾�Ʈ������ ���� ��Ʈ�ѷ��̹Ƿ� �� �Լ����� ���� ������ ������Ʈ�� ó���մϴ�.
    AddLocalDamage(Damage);
}

void ATrainingPlayerController::AddLocalDamage(float DamageAmount)
{
    LocalTotalDamage += DamageAmount;
    // TODO :: ��� ��ã������ DPS ������Ʈ �ȵǴ� ���� ����
    // TODO :: ���� ��Ʈ�ѷ����� ��Ȯ�ϰ� �������ְ� HUD ������ Ȯ��
    if (IsLocalController())
    {
        if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
        {
            if (UTrainingWidget* W = HUD->GetTrainingWidget())
            {
                W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
            }
        }
    }     
}

void ATrainingPlayerController::EndLocalTraining()
{
    GetWorld()->GetTimerManager().ClearTimer(LocalTrainingTimerHandle);

    // ������ UI�� �ݿ�
    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
        if (UTrainingWidget* W = HUD->GetTrainingWidget())
        {
            W->UpdateTimer(0.f);
            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
}