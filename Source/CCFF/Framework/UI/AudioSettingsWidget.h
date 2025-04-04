#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "AudioSettingsWidget.generated.h"

UCLASS()
class CCFF_API UAudioSettingsWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	// Slider Callbacks
	UFUNCTION()
	void OnMasterVolumeChanged(float Value);

	UFUNCTION()
	void OnBGMVolumeChanged(float Value);

	UFUNCTION()
	void OnSFXVolumeChanged(float Value);

	// Mute Toggels
	UFUNCTION()
	void OnMasterMuteChanged(bool bIsChecked);

	UFUNCTION()
	void OnBGMMuteChanged(bool bIsChecked);

	UFUNCTION()
	void OnSFXMuteChanged(bool bIsChecked);

	void ApplyVolume();
	void UpdateSoundClassVolume(USoundClass* SoundClass, float Volume, bool bMuted);

	// Volume Slider
	UPROPERTY(meta = (BindWidget))
	class USlider* MasterVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* BGMVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* SFXVolumeSlider;

	// Mute Checkboxes
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* MasterMuteCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* BGMMuteCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* SFXMuteCheckBox;

	// SoundClass references (assign in editor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundClass* MusicSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundClass* SFXSoundClass;

	// Presumed audio values (for demo/testing)
	float MasterVolume = 1.0f;
	float BGMVolume = 1.0f;
	float SFXVolume = 1.0f;

	bool bMasterMuted = false;
	bool bBGMMuted = false;
	bool bSFXMuted = false;

};
