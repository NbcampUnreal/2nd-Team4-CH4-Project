#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/UI/CountdownWidget.h"

void AArenaModeHUD::BeginPlay()
{
	CountdownWidget = CreateAndAddWidget<UCountdownWidget>(CountdownWidgetClass, 1, ESlateVisibility::Visible);
}

void AArenaModeHUD::ShowCountdownWidget()
{
	if (IsValid(CountdownWidget))
	{
		CountdownWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AArenaModeHUD::HideCountdownWidget()
{
	if (IsValid(CountdownWidget))
	{
		CountdownWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
