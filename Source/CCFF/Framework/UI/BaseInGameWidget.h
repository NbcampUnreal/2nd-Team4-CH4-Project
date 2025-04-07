#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseInGameWidget.generated.h"

class UTextBlock;

UCLASS()
class CCFF_API UBaseInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CCFF|UI")
	void UpdateTimerText(const FString& NewTime);
	virtual void UpdateTimerText_Implementation(const FString& NewTime);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TimerText;
};
