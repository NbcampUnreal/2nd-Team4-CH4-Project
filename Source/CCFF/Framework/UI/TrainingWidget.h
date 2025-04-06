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

protected:
	virtual void NativeConstruct() override;

	float CurrentTime = 0.0f;

	FTimerHandle TimerHandle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimerText;
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TimeInputBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ResetButton;

};
