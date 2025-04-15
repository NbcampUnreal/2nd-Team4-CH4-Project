#include "Framework/GameState/ArenaGameState.h"
#include "Net/UnrealNetwork.h"
#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/UI/CountdownWidget.h"
#include "Framework/UI/BaseInGameWidget.h"

AArenaGameState::AArenaGameState()
	: CountdownTime(0.0f)
	, bIsFinishCountdown(false)
	, ArenaRemainingTime(0.0f)
	, CurrentArenaSubMode(EArenaSubMode::Elimination)
{
	bReplicates = true;

}

void AArenaGameState::InitializeGameState()
{
	Super::InitializeGameState();

	CountdownTime = 0.0f;
	bIsFinishCountdown = false;
	ArenaRemainingTime = 0.0f;
}

void AArenaGameState::SetRoundProgress(ERoundProgress NewProgress)
{
	if (HasAuthority())
	{
		ArenaRoundProgress = NewProgress;
		OnRep_ArenaRoundProgress();
	}
}

void AArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaGameState, ArenaRoundProgress);
	DOREPLIFETIME(AArenaGameState, CountdownTime);
	DOREPLIFETIME(AArenaGameState, bIsFinishCountdown);
	DOREPLIFETIME(AArenaGameState, ArenaRemainingTime);
	DOREPLIFETIME(AArenaGameState, RankingInfos);
	DOREPLIFETIME(AArenaGameState, RoundStartTime);
	DOREPLIFETIME(AArenaGameState, CurrentArenaSubMode);
}

void AArenaGameState::OnRep_ArenaRoundProgress()
{
	UE_LOG(LogTemp, Log, TEXT("[ArenaGameState] ArenaRoundProgress changed to: %d"), static_cast<uint8>(ArenaRoundProgress));
}

void AArenaGameState::OnRep_CountdownTime()
{
	if (CountdownTime <= 0.0f || bIsFinishCountdown)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
		{
			if (APlayerController* MyPlayerController = It->Get())
			{
				if (AArenaModeHUD* ArenaModeHUD = Cast<AArenaModeHUD>(MyPlayerController->GetHUD()))
				{
					if (UCountdownWidget* CountdownWidget = ArenaModeHUD->GetCountdownWidget())
					{
						CountdownWidget->SetCountdownText(TEXT("START"));
					}
				}
			}
		}
		return;
	}

	if (!bIsFinishCountdown)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
		{
			if (APlayerController* MyPlayerController = It->Get())
			{
				if (AArenaModeHUD* ArenaModeHUD = Cast<AArenaModeHUD>(MyPlayerController->GetHUD()))
				{
					if (UCountdownWidget* CountdownWidget = ArenaModeHUD->GetCountdownWidget())
					{
						FString DisplayText = FString::FromInt(FMath::FloorToInt(CountdownTime));
						CountdownWidget->SetCountdownText(DisplayText);
					}
				}
			}
		}
	}
}

void AArenaGameState::OnRep_FinishCountdown()
{
	if (bIsFinishCountdown)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
		{
			if (APlayerController* MyPlayerController = It->Get())
			{
				if (AArenaModeHUD* ArenaModeHUD = Cast<AArenaModeHUD>(MyPlayerController->GetHUD()))
				{
					if (UCountdownWidget* CountdownWidget = ArenaModeHUD->GetCountdownWidget())
					{
						CountdownWidget->SetCountdownText(TEXT("START"));
					}
					ArenaModeHUD->HideCountdownWidget();
				}
			}
		}
	}
}

void AArenaGameState::OnRep_ArenaRemainingTime()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		if (APlayerController* MyPlayerController = It->Get())
		{
			if (ABaseInGameHUD* BaseInGameHUD = Cast<ABaseInGameHUD>(MyPlayerController->GetHUD()))
			{
				if (UBaseInGameWidget* BaseInGameWidget = BaseInGameHUD->GetBaseInGameWidget())
				{
					BaseInGameWidget->UpdateTimerText(ArenaRemainingTime);
				}
			}

			if (AArenaModeHUD* ArenaModeHUD = Cast<AArenaModeHUD>(MyPlayerController->GetHUD()))
			{
				if (UCountdownWidget* CountdownWidget = ArenaModeHUD->GetCountdownWidget())
				{
					if (ArenaRemainingTime <= 0.0f)
					{
						ArenaModeHUD->ShowCountdownWidget();
						CountdownWidget->SetCountdownText(TEXT("FINISH"));

						FTimerHandle ResultTimerHandle;

						GetWorld()->GetTimerManager().SetTimer(ResultTimerHandle, [ArenaModeHUD]()
							{
								ArenaModeHUD->ShowArenaResultWidget();
							}, 3.0f, false);
					}
				}
			}
		}
	}
}
