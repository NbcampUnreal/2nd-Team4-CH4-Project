#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Items/Structure/CustomizationPreset.h"
#include "LockerRoomWidget.generated.h"


DECLARE_DELEGATE(FOnBackToMainMenuRequested);

class UButton;
class UTextBlock;

UCLASS()
class CCFF_API ULockerRoomWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void RequestReturnToMainMenu();
	
	FOnBackToMainMenuRequested OnBackToMainMenuRequested;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* L_PresetButton;
	UPROPERTY(meta = (BindWidget))
	UButton* R_PresetButton;

	UPROPERTY(meta = (BindWidget))
	UButton* L_HeadButton;
	UPROPERTY(meta = (BindWidget))
	UButton* R_HeadButton;
	UPROPERTY(meta = (BindWidget))
	UButton* L_FaceButton;
	UPROPERTY(meta = (BindWidget))
	UButton* R_FaceButton;
	UPROPERTY(meta = (BindWidget))
	UButton* L_ShoulderButton;
	UPROPERTY(meta = (BindWidget))
	UButton* R_ShoulderButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ClearButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PresetNumberText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HeadNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FaceNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShoulderNameText;

	APawn* PreviewPawn;

private:
	UFUNCTION()
	void OnLeftPresetButtonClicked();
	UFUNCTION()
	void OnRightPresetButtonClicked();
	UFUNCTION()
	void OnLeftHeadButtonClicked();
	UFUNCTION()
	void OnRightHeadButtonClicked();
	UFUNCTION()
	void OnLeftFaceButtonClicked();
	UFUNCTION()
	void OnRightFaceButtonClicked();
	UFUNCTION()
	void OnLeftShoulderButtonClicked();
	UFUNCTION()
	void OnRightShoulderButtonClicked();
	UFUNCTION()
	void OnSaveButtonClicked();
	UFUNCTION()
	void OnClearButtonClicked();

	void SaveCurrentPreset(FPresetItemsindex& ItemIndexes);

	void InitializePreviewPawn();

	int32 CurrentPresetIndex = 0;
	int32 CurrentHeadIndex = 0;
	int32 CurrentFaceIndex = 0;
	int32 CurrentShoulderIndex = 0;
};
