#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Components/Border.h"
#include "SettingsWidget.generated.h"

UCLASS()
class CCFF_API USettingsWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	UFUNCTION()
	void OnAudioTabClicked();

	UFUNCTION()
	void OnGraphicTabClicked();

	UFUNCTION()
	void OnControlTabClicked();

	UFUNCTION()
	void OnApplyClicked();

	UFUNCTION()
	void OnResetClicked();

	UFUNCTION()
	void OnCancelClicked();

	void UpdateTabFocus();

	// Tab buttons for category switching
	UPROPERTY(meta = (BindWidget))
	class UBorder* SettingsContentBorder;

	UPROPERTY(meta = (BindWidget))
	class UButton* AudioTabButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GraphicTabButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ControlTabButton;

	// Action buttons for settings management
	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ResetButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	// Widget classes for each settings category
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UAudioSettingsWidget> AudioSettingsWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UGraphicSettingsWidget> GraphicSettingsWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UControlSettingsWidget> ControlSettingsWidgetClass;

private:

	UPROPERTY()
	UUserWidget* CurrentSettingsWidget = nullptr;

	int32 CurrentTabIndex = 0;

	// Tab button list for keyboard navigation
	TArray<UButton*> TabButtons;

	template<typename TWidget>
	void SwitchSettingsWidget(TSubclassOf<TWidget> NewWidgetClass);

};

template<typename TWidget>
inline void USettingsWidget::SwitchSettingsWidget(TSubclassOf<TWidget> NewWidgetClass)
{
	if (!SettingsContentBorder || !NewWidgetClass) return;

	if (CurrentSettingsWidget)
	{
		CurrentSettingsWidget->RemoveFromParent();
		CurrentSettingsWidget = nullptr;
	}

	if (NewWidgetClass)
	{
		CurrentSettingsWidget = CreateWidget<TWidget>(GetWorld(), NewWidgetClass);
		if (CurrentSettingsWidget)
		{
			SettingsContentBorder->SetContent(CurrentSettingsWidget);
		}
	}
}
