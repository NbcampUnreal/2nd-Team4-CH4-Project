#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "LoginWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoginSuccess);

UCLASS()
class CCFF_API ULoginWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnLoginSuccess OnLoginSuccess;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* NicknameTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	class UCCFFGameInstance* GetGameInstanceTyped() const;

};
