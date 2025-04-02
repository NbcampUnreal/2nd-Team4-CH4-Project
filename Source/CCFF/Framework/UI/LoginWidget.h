#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/CCFFBaseUserWidget.h"
#include "LoginWidget.generated.h"

UCLASS()
class CCFF_API ULoginWidget : public UCCFFBaseUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* NicknameTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
