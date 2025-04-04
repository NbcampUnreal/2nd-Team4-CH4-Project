#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class CCFF_API UMainMenuWidget : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void UpdateButtonFocus();
	void HandleEnterPressed();

	UFUNCTION()
	void HandleEnterArenaClicked();

	UFUNCTION()
	void HandleSettingClicked();

	UFUNCTION()
	void HandleExitGameClicked();

	UFUNCTION()
	void HandleExitGameConfirmed();

	UFUNCTION()
	void HandleExitGameCanceled();

	UPROPERTY(meta = (BindWidget))
	class UButton* EnterArenaButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* LockerRoomButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ShopButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UConfirmPopupWidget> ExitGamePopupClass;

	UPROPERTY()
	class UConfirmPopupWidget* ExitGamePopup;

	UPROPERTY()
	TArray<UButton*> MenuButtons;

	UPROPERTY()
	int32 CurrentIndex = 0;
	
};
