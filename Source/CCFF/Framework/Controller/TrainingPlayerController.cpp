#include "Framework/Controller/TrainingPlayerController.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "Framework/GameState/TrainingGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "GameFramework/GameStateBase.h" // 추가된 include
#include <Kismet/GameplayStatics.h>

void ATrainingPlayerController::StartLocalTraining(float InTime)
{
    UE_LOG(LogTemp, Log, TEXT("StartLocalTraining: InTime = %.2f"), InTime);

   // 초기값 세팅
   LocalInitialTime = InTime;
   LocalRemainingTime = InTime;
   LocalTotalDamage = 0.f;
   LocalDPS = 0.f;

   if (ATrainingGameState* TGameState = Cast<ATrainingGameState>(GetWorld()->GetGameState<ATrainingGameState>()))
   {
       TGameState->SetRoundProgress(ERoundProgress::InProgress);
   }

   // 1초마다 UpdateLocalTraining() 호출
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
//    // 남은 시간 감소
//    if (LocalRemainingTime <= 0.f)
//    {
//        EndLocalTraining();
//        return;
//    }
//    LocalRemainingTime -= 1.f;
//
//    // DPS 계산
//    float Elapsed = LocalInitialTime - LocalRemainingTime;
//    LocalDPS = (Elapsed > 0.f) ? (LocalTotalDamage / Elapsed) : 0.f;
//
//    // UI 갱신
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

    // UI 갱신
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
    // 클라이언트에서는 로컬 컨트롤러이므로 이 함수에서 로컬 데미지 업데이트를 처리합니다.
    AddLocalDamage(Damage);
}

void ATrainingPlayerController::AddLocalDamage(float DamageAmount)
{
    LocalTotalDamage += DamageAmount;
    // TODO :: 허드 안찾아져서 DPS 업데이트 안되는 버그 수정
    // TODO :: 로컬 컨트롤러인지 명확하게 지정해주고 HUD 들어가는지 확인
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

    // 최종값 UI에 반영
    if (ATrainingModeHUD* HUD = Cast<ATrainingModeHUD>(GetHUD()))
        if (UTrainingWidget* W = HUD->GetTrainingWidget())
        {
            W->UpdateTimer(0.f);
            W->UpdateTrainingStatsData(LocalTotalDamage, LocalDPS);
        }
}