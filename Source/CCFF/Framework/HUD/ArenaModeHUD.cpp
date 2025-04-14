#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/UI/CountdownWidget.h"
#include "Framework/UI/ArenaResultWidget.h"
#include "Framework/GameState/ArenaGameState.h"

void AArenaModeHUD::BeginPlay()
{
	Super::BeginPlay();

	CountdownWidget = CreateAndAddWidget<UCountdownWidget>(CountdownWidgetClass, 1, ESlateVisibility::Visible);
	ResultWidget = CreateAndAddWidget<UArenaResultWidget>(ResultWidgetClass, 2, ESlateVisibility::Collapsed);
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

void AArenaModeHUD::ShowArenaResultWidget()
{
	if (IsValid(ResultWidget))
	{
		AArenaGameState* ArenaGS = Cast<AArenaGameState>(GetWorld()->GetGameState());
		if (ArenaGS)
		{
			ResultWidget->SetRankingInfos(ArenaGS->RankingInfos);
		}

		ResultWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AArenaModeHUD::HideArenaResultWidget()
{
	if (IsValid(ResultWidget))
	{
		ResultWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
