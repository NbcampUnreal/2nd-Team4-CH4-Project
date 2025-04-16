#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreviewReadyStatusWidget.generated.h"

UCLASS()
class CCFF_API UPreviewReadyStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetReadyState(bool bIsReady);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ReadyStatusText;

};
