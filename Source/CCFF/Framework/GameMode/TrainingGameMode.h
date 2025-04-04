#pragma once

#include "CoreMinimal.h"
#include "Framework/GameMode/BaseInGameMode.h"
#include "TrainingGameMode.generated.h"


UCLASS()
class CCFF_API ATrainingGameMode : public ABaseInGameMode
{
	GENERATED_BODY()

public:
	ATrainingGameMode(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
	void StartTraining();

	UFUNCTION(BlueprintCallable, Category = "CCFF|Training")
	void EndTraining();

	virtual void CheckGameConditions() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CCFF|HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	UPROPERTY()
	UUserWidget* TrainingHUD;
};
