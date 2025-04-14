#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Framework/UI/Settings/ApplySettingsInterface.h"
#include "Framework/Data/AudioSettingsStructs.h"
#include "AudioSettingsWidget.generated.h"

UCLASS()
class CCFF_API UAudioSettingsWidget : public UBaseUserWidget, public IApplySettingsInterface
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void ApplySettings_Implementation() override;
	virtual void ResetSettings_Implementation() override;
	virtual void CancelSettings_Implementation() override;

	UFUNCTION()
	void OnMasterMuteChanged(bool bIsChecked);
	UFUNCTION()
	void OnBGMMuteChanged(bool bIsChecked);
	UFUNCTION()
	void OnSFXMuteChanged(bool bIsChecked);

	UFUNCTION()
	void OnMasterVolumeChanged(float Value);
	UFUNCTION()
	void OnBGMVolumeChanged(float Value);
	UFUNCTION()
	void OnSFXVolumeChanged(float Value);

	UFUNCTION()
	void ApplyAudioToEngine();

protected:
	// Master Volume
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* MasterMuteCheckBox;
	UPROPERTY(meta = (BindWidget))
	class USlider* MasterSlider;

	// BGM
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* BGMMuteCheckBox;
	UPROPERTY(meta = (BindWidget))
	class USlider* BGMSlider;

	// SFX
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* SFXMuteCheckBox;
	UPROPERTY(meta = (BindWidget))
	class USlider* SFXSlider;

private:
	FAudioUserSettings CurrentSettings;
	FAudioUserSettings CachedSettings;
	const FAudioUserSettings DefaultSettings;

	void CacheCurrentSettings();
	void RestoreCachedSettings();
	void UpdateUIFromSettings(const FAudioUserSettings& Settings);
};
