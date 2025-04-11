#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TrainingPlayerController.generated.h"


UCLASS()
class CCFF_API ATrainingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    // �������� ȣ��: ���� �Ʒ� ����
    UFUNCTION(BlueprintCallable, Category = "Training")
    void StartLocalTraining(float InTime);

    // �������� ȣ��: ���� �Ʒ� ����
    UFUNCTION(BlueprintCallable, Category = "Training")
    void EndLocalTraining();

    // �Ʒÿ� ���� �¾��� �� ȣ��
    UFUNCTION(BlueprintCallable, Category = "Training")
    void AddLocalDamage(float DamageAmount);
	
protected:
    // �� �� ȣ��� ���� �Լ�
    void UpdateLocalTraining();

    // ���� Ÿ�̸� �ڵ�
    FTimerHandle LocalTrainingTimerHandle;

    // ���� ����
    float LocalRemainingTime;
    float LocalTotalDamage;
    float LocalDPS;
};
