#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "CheckPopupWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckPopupConfirmed);

UCLASS()
class CCFF_API UCheckPopupWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetMessage(const FText& NewMessage);

	UPROPERTY(BlueprintAssignable)
	FOnCheckPopupConfirmed OnCheckPopupConfirmed;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConfirm();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageText;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

};
