#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Framework/UI/Settings/ApplySettingsInterface.h"
#include "GraphicSettingsWidget.generated.h"

UCLASS(config = GameUserSettings)
class CCFF_API UGraphicSettingsWidget : public UBaseUserWidget, public IApplySettingsInterface
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void ApplySettings_Implementation() override;
	virtual void ResetSettings_Implementation() override;
	virtual void CancelSettings_Implementation() override;

	UFUNCTION()
	void OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (BindWidget))
	class UComboBoxString* WindowModeComboBox;

	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (BindWidget))
	class UComboBoxString* ResolutionComboBox;

private:
	// Current Selected Value
	EWindowMode::Type SelectedWindowMode;
	FIntPoint SelectedResolution;

	// Cached Value (Cancel)
	FIntPoint CachedResolution;
	EWindowMode::Type CachedWindowMode;

	// Default Value (Reset)
	const FIntPoint DefaultResolution = FIntPoint(1920, 1080);
	const EWindowMode::Type DefaultWindowMode = EWindowMode::WindowedFullscreen;

	void InitializeWindowModeOptions();
	void InitializeResolutionOptions();
	void UpdateComboBoxSelections();

	void ApplyPreviewSettings();
	void CacheCurrentSettings();
	void RestoreCachedSettings();

};
