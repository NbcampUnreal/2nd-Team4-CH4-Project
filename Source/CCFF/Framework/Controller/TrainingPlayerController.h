#pragma once

#include "CoreMinimal.h"
#include "Character/Base/CharacterController.h"
#include "TrainingPlayerController.generated.h"


UCLASS()
class CCFF_API ATrainingPlayerController : public ACharacterController
{
	GENERATED_BODY()

public:
    // �������� ȣ��: ���� �Ʒ� ����
    UFUNCTION(BlueprintCallable, Category = "Training")
    void StartLocalTraining(float InTime);

    void AddLocalDamage(float DamageAmount);

    // �������� ȣ��: ���� �Ʒ� ����
    UFUNCTION(BlueprintCallable, Category = "Training")
    void EndLocalTraining();

    // �Ʒÿ� ���� �¾��� �� ȣ��
    UFUNCTION(Client, Reliable)
    void ClientAddLocalDamage(float Damage);
	
protected:
    // �� �� ȣ��� ���� �Լ�
    void UpdateLocalTraining();

    // ���� Ÿ�̸� �ڵ�
    FTimerHandle LocalTrainingTimerHandle;

    // ���� ����
    float LocalInitialTime;
    float LocalRemainingTime;
    float LocalTotalDamage;
    float LocalDPS;
};
