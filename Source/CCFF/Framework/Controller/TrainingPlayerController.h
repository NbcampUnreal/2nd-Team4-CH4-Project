#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TrainingPlayerController.generated.h"


UCLASS()
class CCFF_API ATrainingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    // 위젯에서 호출: 로컬 훈련 시작
    UFUNCTION(BlueprintCallable, Category = "Training")
    void StartLocalTraining(float InTime);

    // 위젯에서 호출: 로컬 훈련 종료
    UFUNCTION(BlueprintCallable, Category = "Training")
    void EndLocalTraining();

    // 훈련용 봇이 맞았을 때 호출
    UFUNCTION(BlueprintCallable, Category = "Training")
    void AddLocalDamage(float DamageAmount);
	
protected:
    // 매 초 호출될 내부 함수
    void UpdateLocalTraining();

    // 내부 타이머 핸들
    FTimerHandle LocalTrainingTimerHandle;

    // 로컬 변수
    float LocalRemainingTime;
    float LocalTotalDamage;
    float LocalDPS;
};
