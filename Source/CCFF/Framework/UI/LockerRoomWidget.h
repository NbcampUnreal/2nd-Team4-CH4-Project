#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Items/Structure/CustomizationPreset.h"
#include "LockerRoomWidget.generated.h"


DECLARE_DELEGATE(FOnBackToMainMenuRequested);

class UButton;
class UTextBlock;
class UCustomizationManager;
class ABasePreviewPawn;

UCLASS()
class CCFF_API ULockerRoomWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void HandleCharacterChanged();
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

	ABasePreviewPawn* PreviewPawn;

private:
	UCustomizationManager* CustomizationManager;
	
	int32 TotalHeadNums = 0;
	int32 TotalFaceNums = 0;
	int32 TotalShoulderNums = 0;

	int32 CurrentPresetIndex = 0;
	int32 CurrentHeadIndex = 0;
	int32 CurrentFaceIndex = 0;
	int32 CurrentShoulderIndex = 0;

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

	void CountTotalItemNums();

	void UpdatePresetNumberText();
	void UpdateHeadNameText();
	void UpdateFaceNameText();
	void UpdateShoulderNameText();
	void UpdateIndexText();

	void LoadPreset(int32 PresetIndex);

	void InitializePreviewPawn();
	void InitializeCustomizationManager();
	void BindToCharacterChangedEvent();
};
