#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/UI/CountdownWidget.h"
#include "Framework/UI/ArenaResultWidget.h"
#include "Framework/GameState/ArenaGameState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

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

void AArenaModeHUD::UpdateCountdownText(const FString& InText)
{
	if (CountdownWidget)
		CountdownWidget->SetCountdownText(InText);
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

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(ResultWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
}

void AArenaModeHUD::HideArenaResultWidget()
{
	if (IsValid(ResultWidget))
	{
		ResultWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
