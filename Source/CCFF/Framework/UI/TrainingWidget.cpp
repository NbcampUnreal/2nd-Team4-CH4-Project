#include "Framework/UI/TrainingWidget.h"
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

	if (IsValid(TimerText))
	{
		TimerText->SetText(FText::FromString(TEXT("00:00")));
	}
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

		if (IsValid(TimerText))
		{
			TimerText->SetText(FText::FromString(FormattedTime));
		}

		// initialize timer
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTrainingWidget::UpdateTimer, 1.0f, true);
	}
}

void UTrainingWidget::OnResetButtonClicked()
{
	if (IsValid(TimerText))
	{
		TimerText->SetText(FText::FromString(TEXT("00:00")));
	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

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

	if (IsValid(TimerText))
	{
		TimerText->SetText(FText::FromString(FormattedTime));
	}
}

