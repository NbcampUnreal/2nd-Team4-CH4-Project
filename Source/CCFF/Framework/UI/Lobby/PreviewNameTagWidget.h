#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "PreviewNameTagWidget.generated.h"

UCLASS()
class CCFF_API UPreviewNameTagWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerName(const FString& NewName);

	UFUNCTION(BlueprintCallable)
	void SetHostMarkVisible(bool bVisible);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	class UImage* HostImage;

};
