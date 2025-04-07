#pragma once

#include "CoreMinimal.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "TrainingModeHUD.generated.h"

class UTrainingWidget;

UCLASS()
class CCFF_API ATrainingModeHUD : public ABaseInGameHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void ShowTrainingWidget();

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	FORCEINLINE UTrainingWidget* GetTrainingWidget() const { return TrainingWidget; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<UTrainingWidget> TrainingWidgetClass;

	UPROPERTY()
	UTrainingWidget* TrainingWidget;
};
