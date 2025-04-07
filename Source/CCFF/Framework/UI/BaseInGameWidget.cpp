#include "Framework/UI/BaseInGameWidget.h"
#include "Components/TextBlock.h"

void UBaseInGameWidget::UpdateTimerText_Implementation(const FString& NewTime)
{
	if (TimerText)
	{
		TimerText->SetText(FText::FromString(NewTime));
	}
}
