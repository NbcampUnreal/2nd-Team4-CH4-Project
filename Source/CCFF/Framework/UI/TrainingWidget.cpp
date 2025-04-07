#include "Framework/UI/TrainingWidget.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "TimeManagementClasses.h"


void UTrainingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(StartButton))
	{
		StartButton->OnClicked.AddDynamic(this, &UTrainingWidget::OnStartButtonClicked);
	}

	if (IsValid(ResetButton))
	{
		ResetButton->OnClicked.AddDynamic(this, &UTrainingWidget::OnResetButtonClicked);
	}

	UpdateTimerText(TEXT("00:00"));
}


void UTrainingWidget::OnStartButtonClicked()
{
	if (IsValid(TimeInputBox))
	{
		const FString InputStr = TimeInputBox->GetText().ToString();
		float EnterTime = FCString::Atof(*InputStr);

		if (EnterTime < 0.f)
			EnterTime = 0.f;

		CurrentTime = EnterTime;

		int32 Minutes = FMath::FloorToInt(CurrentTime / 60.0f);
		int32 Seconds = FMath::FloorToInt(CurrentTime) % 60;
		FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

		if (APlayerController* PC = GetOwningPlayer())
		{
			if (ABaseInGameHUD* HUD = Cast<ABaseInGameHUD>(PC->GetHUD()))
			{
				if (UBaseInGameWidget* BaseWidget = HUD->GetBaseInGameWidget())
				{
					BaseWidget->UpdateTimerText(FormattedTime);
				}
			}
		}

		// initialize timer
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTrainingWidget::UpdateTimer, 1.0f, true);
	}
}

void UTrainingWidget::OnResetButtonClicked()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	CurrentTime = 0.0f;
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ABaseInGameHUD* HUD = Cast<ABaseInGameHUD>(PC->GetHUD()))
		{
			if (UBaseInGameWidget* BaseWidget = HUD->GetBaseInGameWidget())
			{
				BaseWidget->UpdateTimerText(TEXT("00:00"));
			}
		}
	}

	// TODO : Add Initialize Total damame, max combo, dps, TimeText 60
}

void UTrainingWidget::UpdateTimer()
{

	CurrentTime -= 1.0f;

	if (CurrentTime <= 0.0f)
	{
		CurrentTime = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	int32 Minutes = FMath::FloorToInt(CurrentTime / 60.0f);
	int32 Seconds = FMath::FloorToInt(CurrentTime) % 60;
	FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ABaseInGameHUD* HUD = Cast<ABaseInGameHUD>(PC->GetHUD()))
		{
			if (UBaseInGameWidget* BaseWidget = HUD->GetBaseInGameWidget())
			{
				BaseWidget->UpdateTimerText(FormattedTime);
			}
		}
	}
}

void UTrainingWidget::UpdateTrainingStatsData(float TotalDamage, int32 MaxCombo, float DPS)
{
	if (TotalDamageText)
	{
		TotalDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), TotalDamage)));
	}
	if (MaxComboText)
	{
		MaxComboText->SetText(FText::FromString(FString::FromInt(MaxCombo)));
	}
	if (DPSText)
	{
		DPSText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), DPS)));
	}
}

