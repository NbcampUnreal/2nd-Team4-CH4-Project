#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "QuitPopupWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitPopupConfirmed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitPopupCanceled);

UCLASS()
class CCFF_API UQuitPopupWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnQuitPopupConfirmed OnQuitConfirmed;

	UPROPERTY(BlueprintAssignable)
	FOnQuitPopupCanceled OnQuitCanceled;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnCancelClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
	
};
