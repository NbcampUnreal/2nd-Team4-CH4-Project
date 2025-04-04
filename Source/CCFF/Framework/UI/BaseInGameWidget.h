#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseInGameWidget.generated.h"


UCLASS()
class CCFF_API UBaseInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "CCFF|HUD")
	void UpdatePlayerInfo(int Health, float RemainingTime);
	virtual void UpdatePlayerInfo_Implementation(int Health, float RemainingTime);
};
