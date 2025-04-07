#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "SelectModeWidget.generated.h"

UCLASS()
class CCFF_API USelectModeWidget : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnAdventrueClicked();

	UFUNCTION()
	void OnArenaClicked();

	UFUNCTION()
	void OnTrainingRoomClicked();

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void HandleTrainingPopupConfirmed();

	UFUNCTION()
	void HandleTrainingPopupCanceled();

	UPROPERTY(meta = (BindWidget))
	class UButton* AdventureButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ArenaButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* TrainingRoomButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAcess = "true"))
	TSubclassOf<class UConfirmPopupWidget> ConfirmPopupClass;

	UPROPERTY()
	class UConfirmPopupWidget* TrainingRoomPopup;

};
