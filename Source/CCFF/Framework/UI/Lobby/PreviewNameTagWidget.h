#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreviewNameTagWidget.generated.h"

UCLASS()
class CCFF_API UPreviewNameTagWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerName(const FString& NewName);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;

};
