#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseInGameWidget.generated.h"

class UStatusComponent;
class UProfileWidget;
class USideBarWidget;
class UTextBlock;

UCLASS()
class CCFF_API UBaseInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CCFF|UI")
	void UpdateTimerText(const float NewTime);
	virtual void UpdateTimerText_Implementation(const float NewTime);

	void UpdateHealthBar(const float InPercentage);
	void UpdateSuperMeterBar(const float InPercentage);
	void UpdateBurstMeterBar(const float InPercentage);

	void InitializeHUDWidget(UStatusComponent* InStatusComponent);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TimerText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProfileWidget> ProfileWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<USideBarWidget> SideBarWidget;
};
