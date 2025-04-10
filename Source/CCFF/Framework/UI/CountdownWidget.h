#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "CountdownWidget.generated.h"


UCLASS()
class CCFF_API UCountdownWidget : public UBaseInGameWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetCountdownText(FString StringTime);
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountdownText;

};
