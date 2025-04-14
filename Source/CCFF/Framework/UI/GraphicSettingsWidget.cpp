#include "Framework/UI/GraphicSettingsWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"
#include "Framework/Application/SlateApplication.h"

void UGraphicSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWindowModeOptions();
	InitializeResolutionOptions();

	CachedResolution = SelectedResolution;
	CachedWindowMode = SelectedWindowMode;
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

void UGraphicSettingsWidget::ApplySettings_Implementation()
{
	CachedResolution = SelectedResolution;
	CachedWindowMode = SelectedWindowMode;
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
	}
}

void UGraphicSettingsWidget::RestoreCachedSettings()
{
	SelectedResolution = CachedResolution;
	SelectedWindowMode = CachedWindowMode;

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetScreenResolution(CachedResolution);
		Settings->SetFullscreenMode(CachedWindowMode);
		Settings->ApplySettings(false);
	}

	UpdateComboBoxSelections();
}

void UGraphicSettingsWidget::UpdateComboBoxSelections()
{
	if (ResolutionComboBox)
	{
		FString ResText = FString::Printf(TEXT("%dx%d"), CachedResolution.X, CachedResolution.Y);
		ResolutionComboBox->SetSelectedOption(ResText);
	}

	if (WindowModeComboBox)
	{
		FString ModeText = TEXT("Windowed");
		if (CachedWindowMode == EWindowMode::WindowedFullscreen)
			ModeText = TEXT("Fullscreen");
		else if (CachedWindowMode == EWindowMode::Fullscreen)
			ModeText = TEXT("Borderless");

		WindowModeComboBox->SetSelectedOption(ModeText);
	}
}