#pragma once

#include "CoreMinimal.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "TrainingHUD.generated.h"

UCLASS()
class CCFF_API ATrainingHUD : public ABaseInGameHUD
{
GENERATED_BODY()

public:
UFUNCTION(BlueprintNativeEvent, Category = "CCFF|TrainingHUD")
void UpdateTrainingStats(float TotalDamage, int32 MaxCombo, float DPS);
virtual void UpdateTrainingStats_Implementation(float TotalDamage, int32 MaxCombo, float DPS);
};
