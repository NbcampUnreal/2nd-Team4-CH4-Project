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

	void StartRound() override;
	void EndRound() override;
	virtual void CheckGameConditions() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CCFF|HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	UPROPERTY()
	UUserWidget* TrainingHUD;
};
