#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "PreviewReadyStatusWidget.generated.h"

UCLASS()
class CCFF_API UPreviewReadyStatusWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetReadyState(bool bIsReady);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ReadyStatusText;

};
