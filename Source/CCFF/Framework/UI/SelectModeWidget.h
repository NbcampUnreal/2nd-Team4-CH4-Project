#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "SelectModeWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArenaModeRequested);

UCLASS()
class CCFF_API USelectModeWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnArenaModeRequested OnArenaModeRequested;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnAdventureClicked();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCheckPopupWidget> CheckPopupClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAcess = "true"))
	TSubclassOf<class UConfirmPopupWidget> ConfirmPopupClass;

	UPROPERTY()
	class UCheckPopupWidget* AdventureModePopup;

	UPROPERTY()
	class UConfirmPopupWidget* TrainingRoomPopup;

};
