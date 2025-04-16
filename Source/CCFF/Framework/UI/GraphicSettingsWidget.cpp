#include "Framework/UI/GraphicSettingsWidget.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"
#include "Framework/Application/SlateApplication.h"

void UGraphicSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWindowModeOptions();
	InitializeResolutionOptions();

	if (UCCFFGameInstance* CCFFGameInstance = GetWorld()->GetGameInstance<UCCFFGameInstance>())
	{
		SelectedResolution = CCFFGameInstance->GetPlayerMeta().Resolution;
		SelectedWindowMode = CCFFGameInstance->GetPlayerMeta().WindowMode;

		UpdateComboBoxSelections();
		CacheCurrentSettings();
	}
}

void UGraphicSettingsWidget::InitializeWindowModeOptions()
{
	if (IsValid(WindowModeComboBox) == false)
	{
		return;
	}

	WindowModeComboBox->ClearOptions();
	WindowModeComboBox->AddOption(TEXT("Fullscreen"));
	WindowModeComboBox->AddOption(TEXT("Windowed"));
	WindowModeComboBox->AddOption(TEXT("Borderless"));

	WindowModeComboBox->SetSelectedOption(TEXT("Borderless"));
	WindowModeComboBox->OnSelectionChanged.AddDynamic(this, &UGraphicSettingsWidget::OnWindowModeChanged);

	SelectedWindowMode = DefaultWindowMode;
}

void UGraphicSettingsWidget::InitializeResolutionOptions()
{
	if (IsValid(ResolutionComboBox) == false)
	{
		return;
	}

	TArray<FString> ResOptions = {
		TEXT("1920x1080"),
		TEXT("1600x900"),
		TEXT("1280x720"),
		TEXT("1024x576")
	};

	for (const FString& Option : ResOptions)
	{
		ResolutionComboBox->AddOption(Option);
	}

	ResolutionComboBox->SetSelectedOption(TEXT("1280x720"));
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UGraphicSettingsWidget::OnResolutionChanged);

	SelectedResolution = DefaultResolution;
}

void UGraphicSettingsWidget::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "Fullscreen")
	{
		SelectedWindowMode = EWindowMode::WindowedFullscreen;
	}
	else if (SelectedItem == "Windowed")
	{
		SelectedWindowMode = EWindowMode::Windowed;
	}
	else if (SelectedItem == "Borderless")
	{
		SelectedWindowMode = EWindowMode::Fullscreen;
	}

	ApplyPreviewSettings();
}

void UGraphicSettingsWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString Width, Height;
	if (SelectedItem.Split(TEXT("x"), &Width, &Height))
	{
		SelectedResolution.X = FCString::Atoi(*Width);
		SelectedResolution.Y = FCString::Atoi(*Height);
	}

	ApplyPreviewSettings();
}

void UGraphicSettingsWidget::ApplyPreviewSettings()
{
	if (SelectedWindowMode == EWindowMode::WindowedFullscreen)
	{
		FDisplayMetrics Metrics;
		FDisplayMetrics::RebuildDisplayMetrics(Metrics);
		SelectedResolution = FIntPoint(Metrics.PrimaryDisplayWidth, Metrics.PrimaryDisplayHeight);
	}

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetScreenResolution(SelectedResolution);
		Settings->SetFullscreenMode(SelectedWindowMode);
		Settings->ApplySettings(false);

		Settings->RequestResolutionChange(SelectedResolution.X, SelectedResolution.Y, SelectedWindowMode);
	}
}

void UGraphicSettingsWidget::ApplySettings_Implementation()
{
	CacheCurrentSettings();

	if (UCCFFGameInstance* CCFFGameInstance = GetWorld()->GetGameInstance<UCCFFGameInstance>())
	{
		FPlayerMetaData Meta = CCFFGameInstance->GetPlayerMeta();
		Meta.Resolution = SelectedResolution;
		Meta.WindowMode = SelectedWindowMode;
		CCFFGameInstance->ApplyUserSettings(Meta);
	}
}

void UGraphicSettingsWidget::ResetSettings_Implementation()
{
	SelectedResolution = DefaultResolution;
	SelectedWindowMode = DefaultWindowMode;

	if (ResolutionComboBox)
	{
		ResolutionComboBox->SetSelectedOption(TEXT("1280x720"));
	}
	if (WindowModeComboBox)
	{
		WindowModeComboBox->SetSelectedOption(TEXT("Borderless"));
	}

	ApplyPreviewSettings();
}

void UGraphicSettingsWidget::CancelSettings_Implementation()
{
	RestoreCachedSettings();
	UpdateComboBoxSelections();
	ApplyPreviewSettings();
}

void UGraphicSettingsWidget::CacheCurrentSettings()
{
	CachedResolution = SelectedResolution;
	CachedWindowMode = SelectedWindowMode;
}

void UGraphicSettingsWidget::RestoreCachedSettings()
{
	SelectedResolution = CachedResolution;
	SelectedWindowMode = CachedWindowMode;
}

void UGraphicSettingsWidget::UpdateComboBoxSelections()
{
	if (ResolutionComboBox)
	{
		FString ResText = FString::Printf(TEXT("%dx%d"), SelectedResolution.X, SelectedResolution.Y);
		ResolutionComboBox->SetSelectedOption(ResText);
	}

	if (WindowModeComboBox)
	{
		FString ModeText = TEXT("Windowed");
		if (SelectedWindowMode == EWindowMode::WindowedFullscreen)
			ModeText = TEXT("Fullscreen");
		else if (SelectedWindowMode == EWindowMode::Fullscreen)
			ModeText = TEXT("Borderless");

		WindowModeComboBox->SetSelectedOption(ModeText);
	}
}