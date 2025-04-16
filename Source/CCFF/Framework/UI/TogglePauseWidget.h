#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TogglePauseWidget.generated.h"

class UButton;
class UConfirmPopupWidget;

UCLASS()
class CCFF_API UTogglePauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UTogglePauseWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnSettingButtonClicked();

	UFUNCTION()
	void OnLobbyButtonClicked();

	UFUNCTION()
	void OnMoveLobbyConfirmed();

	UFUNCTION()
	void OnMoveLobbyCanceled();

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LobbyButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UConfirmPopupWidget> MoveLobbyPopupClass;

	UPROPERTY()
	UConfirmPopupWidget* MoveLobbyPopup;
};
