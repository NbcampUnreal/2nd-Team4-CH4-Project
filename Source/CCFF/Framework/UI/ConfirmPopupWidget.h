#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "ConfirmPopupWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmPopupConfirmed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmPopupCanceled);

UCLASS()
class CCFF_API UConfirmPopupWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetMessage(const FText& NewMessage);

	UPROPERTY(BlueprintAssignable)
	FOnConfirmPopupConfirmed OnConfirmPopupConfirmed;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmPopupCanceled OnConfirmPopupCanceled;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConfirm();

	UFUNCTION()
	void OnCancel();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageText;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
	
};
