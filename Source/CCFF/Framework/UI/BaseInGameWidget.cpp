#include "Framework/UI/BaseInGameWidget.h"
#include "Components/TextBlock.h"


void UBaseInGameWidget::UpdateTimerText_Implementation(const float NewTime)
{
	int32 Minutes = FMath::FloorToInt(NewTime / 60.0f);
	int32 Seconds = FMath::FloorToInt(NewTime) % 60;
	FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	if (TimerText)
	{
		TimerText->SetText(FText::FromString(FormattedTime));
	}
}
