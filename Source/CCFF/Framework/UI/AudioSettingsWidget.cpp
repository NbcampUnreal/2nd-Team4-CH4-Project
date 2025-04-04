#include "Framework/UI/AudioSettingsWidget.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"

void UAudioSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MasterVolumeSlider)
	{
		MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingsWidget::OnMasterVolumeChanged);
	}

	if (BGMVolumeSlider)
	{
		BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingsWidget::OnBGMVolumeChanged);
	}

	if (SFXVolumeSlider)
	{
		SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingsWidget::OnSFXVolumeChanged);
	}

	if (MasterMuteCheckBox)
	{
		MasterMuteCheckBox->OnCheckStateChanged.AddDynamic(this, &UAudioSettingsWidget::OnMasterMuteChanged);
	}

	if (BGMMuteCheckBox)
	{
		BGMMuteCheckBox->OnCheckStateChanged.AddDynamic(this, &UAudioSettingsWidget::OnBGMMuteChanged);
	}

	if (SFXMuteCheckBox)
	{
		SFXMuteCheckBox->OnCheckStateChanged.AddDynamic(this, &UAudioSettingsWidget::OnSFXMuteChanged);
	}

	// Initial apply
	ApplyVolume();
}

void UAudioSettingsWidget::UpdateSoundClassVolume(USoundClass* SoundClass, float Volume, bool bMuted)
{
	if (SoundClass)
	{
		SoundClass->Properties.Volume = bMuted ? 0.0f : Volume;
	}
}

void UAudioSettingsWidget::ApplyVolume()
{
	float MasteredBGMVolume = MasterVolume * BGMVolume;
	float MasteredSFXVolume = MasterVolume * SFXVolume;

	if (MusicSoundClass)
	{
		UpdateSoundClassVolume(MusicSoundClass, MasteredBGMVolume, bMasterMuted || bBGMMuted);
	}
	
	if (SFXSoundClass)
	{
		UpdateSoundClassVolume(SFXSoundClass, MasteredSFXVolume, bMasterMuted || bSFXMuted);
	}
}

void UAudioSettingsWidget::OnMasterVolumeChanged(float Value)
{
	MasterVolume = Value;
	ApplyVolume();
}

void UAudioSettingsWidget::OnBGMVolumeChanged(float Value)
{
	BGMVolume = Value;
	ApplyVolume();
}

void UAudioSettingsWidget::OnSFXVolumeChanged(float Value)
{
	SFXVolume = Value;
	ApplyVolume();
}

void UAudioSettingsWidget::OnMasterMuteChanged(bool bIsChecked)
{
	bMasterMuted = bIsChecked;
	ApplyVolume();
}

void UAudioSettingsWidget::OnBGMMuteChanged(bool bIsChecked)
{
	bBGMMuted = bIsChecked;
	ApplyVolume();
}

void UAudioSettingsWidget::OnSFXMuteChanged(bool bIsChecked)
{
	bSFXMuted = bIsChecked;
	ApplyVolume();
}
