#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "TrainingWidget.generated.h"

UCLASS()
class CCFF_API UTrainingWidget : public UBaseInGameWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnResetButtonClicked();

	UFUNCTION()
	void UpdateTimer();

	UFUNCTION(BlueprintCallable, Category = "Training Stats")
	void UpdateTrainingStatsData(float TotalDamage, int32 MaxCombo, float DPS);

protected:
	virtual void NativeConstruct() override;

#pragma region SetTimer
	float CurrentTime = 0.0f;

	FTimerHandle TimerHandle;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TimeInputBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ResetButton;
#pragma endregion

#pragma region DPS
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalDamageText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxComboText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DPSText;

#pragma endregion
};
