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
#pragma region Constructor & Overrides
	ULoginWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;
#pragma endregion

	UPROPERTY(BlueprintAssignable)
	FOnLoginSuccess OnLoginSuccess;

protected:
#pragma region Widget Binds
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UEditableText> NicknameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> LoginButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ExitButton;
#pragma endregion

#pragma region Popup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UConfirmPopupWidget> ExitGamePopupClass;

	UPROPERTY()
	class UConfirmPopupWidget* ExitGamePopup;
#pragma endregion

#pragma region UI Event Handlers
	UFUNCTION()
	void OnLoginButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void HandleExitGameConfirmed();
	void HandleExitGameCanceled();
#pragma endregion

private:
#pragma region Validation & Utility
	bool IsNicknameValid(const FString& Nickname, FString& OutErrorMessage) const;
	void ShowErrorPopup(const FString& Message);
#pragma endregion

};
