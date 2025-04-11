#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseInGameWidget.generated.h"

class UProfileWidget;
class USideBarWidget;
class UTextBlock;

UCLASS()
class CCFF_API UBaseInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CCFF|UI")
	void UpdateTimerText(const float NewTime);
	virtual void UpdateTimerText_Implementation(const float NewTime);

	void UpdateHealthBar(const float InPercentage);
	void UpdateSuperMeterBar(const float InPercentage);
	void UpdateBurstMeterBar(const float InPercentage);
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TimerText;
	UPROPERTY(meta = (BindWidgetOptional))
	UProfileWidget* ProfileWidget;
	UPROPERTY(meta = (BindWidgetOptional))
	USideBarWidget* SideBarWidget;
};
