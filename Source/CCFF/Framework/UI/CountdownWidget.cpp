#include "Framework/UI/CountdownWidget.h"
#include "Components/TextBlock.h"

void UCountdownWidget::SetCountdownText(FString StringTime)
{
	CountdownText->SetText(FText::FromString(StringTime));
}
