#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

UCLASS()
class CCFF_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayClickSound();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void BindAllButtonsWithClickSound();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> ClickSound;

};
