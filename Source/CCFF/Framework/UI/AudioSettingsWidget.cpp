#include "Framework/UI/AudioSettingsWidget.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"

void UAudioSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MasterMuteCheckBox)	MasterMuteCheckBox->OnCheckStateChanged.AddDynamic(this, &UAudioSettingsWidget::OnMasterMuteChanged);
	if (BGMMuteCheckBox)	BGMMuteCheckBox->OnCheckStateChanged.AddDynamic(this, &UAudioSettingsWidget::OnBGMMuteChanged);
	if (SFXMuteCheckBox)	SFXMuteCheckBox->OnCheckStateChanged.AddDynamic(this, &UAudioSettingsWidget::OnSFXMuteChanged);

	if (MasterSlider)	MasterSlider->OnValueChanged.AddDynamic(this, &UAudioSettingsWidget::OnMasterVolumeChanged);
	if (BGMSlider)		BGMSlider->OnValueChanged.AddDynamic(this, &UAudioSettingsWidget::OnBGMVolumeChanged);
	if (SFXSlider)		SFXSlider->OnValueChanged.AddDynamic(this, &UAudioSettingsWidget::OnSFXVolumeChanged);

	// Initialize Default Values
	CurrentSettings = DefaultSettings;
	UpdateUIFromSettings(CurrentSettings);
	CacheCurrentSettings();
}

void UAudioSettingsWidget::ApplySettings_Implementation()
{
	CacheCurrentSettings();
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::ResetSettings_Implementation()
{
	CurrentSettings = DefaultSettings;

	UpdateUIFromSettings(CurrentSettings);
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::CancelSettings_Implementation()
{
	RestoreCachedSettings();

	UpdateUIFromSettings(CurrentSettings);
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::OnMasterMuteChanged(bool bIsChecked)
{
	CurrentSettings.Master.bMuted = bIsChecked;
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::OnBGMMuteChanged(bool bIsChecked)
{
	CurrentSettings.BGM.bMuted = bIsChecked;
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::OnSFXMuteChanged(bool bIsChecked)
{
	CurrentSettings.SFX.bMuted = bIsChecked;
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::OnMasterVolumeChanged(float Value)
{
	CurrentSettings.Master.Volume = Value;
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::OnBGMVolumeChanged(float Value)
{
	CurrentSettings.BGM.Volume = Value;
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::OnSFXVolumeChanged(float Value)
{
	CurrentSettings.SFX.Volume = Value;
	ApplyAudioToEngine();
}

void UAudioSettingsWidget::ApplyAudioToEngine()
{
	static USoundMix* Mix = LoadObject<USoundMix>(nullptr, TEXT("/Game/CCFF/Sounds/SoundMixes/MasterMix.MasterMix"));
	if (!Mix) return;

	USoundClass* MasterClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/CCFF/Sounds/SoundClasses/SC_Master.SC_Master"));
	USoundClass* BGMClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/CCFF/Sounds/SoundClasses/SC_BGM.SC_BGM"));
	USoundClass* SFXClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/CCFF/Sounds/SoundClasses/SC_SFX.SC_SFX"));

	auto ApplyMix = [&](USoundClass* SoundClass, const FAudioCategorySetting& Setting)
		{
			if (!SoundClass) return;

			float FinalVolume = Setting.bMuted ? 0.0f : Setting.Volume;

			UGameplayStatics::SetSoundMixClassOverride(
				this, Mix, SoundClass, FinalVolume, 1.0f, 0.0f, true
			);
		};

	ApplyMix(MasterClass, CurrentSettings.Master);
	ApplyMix(BGMClass, CurrentSettings.BGM);
	ApplyMix(SFXClass, CurrentSettings.SFX);

	UGameplayStatics::ClearSoundMixModifiers(this);
	UGameplayStatics::PushSoundMixModifier(this, Mix);
}

void UAudioSettingsWidget::CacheCurrentSettings()
{
	CachedSettings = CurrentSettings;
}

void UAudioSettingsWidget::RestoreCachedSettings()
{
	CurrentSettings = CachedSettings;
}

void UAudioSettingsWidget::UpdateUIFromSettings(const FAudioUserSettings& Settings)
{
	if (MasterMuteCheckBox) MasterMuteCheckBox->SetIsChecked(Settings.Master.bMuted);
	if (BGMMuteCheckBox) BGMMuteCheckBox->SetIsChecked(Settings.BGM.bMuted);
	if (SFXMuteCheckBox) SFXMuteCheckBox->SetIsChecked(Settings.SFX.bMuted);

	if (MasterSlider) MasterSlider->SetValue(Settings.Master.Volume);
	if (BGMSlider) BGMSlider->SetValue(Settings.BGM.Volume);
	if (SFXSlider) SFXSlider->SetValue(Settings.SFX.Volume);
}
