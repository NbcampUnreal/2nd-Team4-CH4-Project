#include "Framework/GameMode/ArenaGameMode.h"
#include "Framework/GameState/ArenaGameState.h"
#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/CountdownWidget.h"


AArenaGameMode::AArenaGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = AArenaGameState::StaticClass();

	MyClassName = "ArenaMode";
	RoundTime = 60.0f;  // Default
	CountdownTime = 3.0f;
}

void AArenaGameMode::BeginPlay()
{
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AArenaGameMode::UpdateCountdown, 1.0f, true);
}

void AArenaGameMode::StartRound()
{
	if (!HasAuthority()) { return; }

	Super::StartRound();

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->InitializeGameState();
		ArenaGameState->SetRoundProgress(ERoundProgress::InProgress);
		ArenaGameState->SetRemainingTime(RoundTime);
	}

	GetWorld()->GetTimerManager().SetTimer(ArenaTimerHandle, this, &AArenaGameMode::UpdateArenaStats, 1.0f, true);
}



void AArenaGameMode::InternalStartRound()
{

	APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(MyPlayerController))
	{
		if (AArenaModeHUD* ArenaModeHUD = Cast<AArenaModeHUD>(MyPlayerController->GetHUD()))
		{
			// 위젯 숨기기
			ArenaModeHUD->HideCountdownWidget();
		}
	}

	StartRound();
}


void AArenaGameMode::EndRound()
{
	if (!HasAuthority()) { return; }

	Super::EndRound();

	GetWorld()->GetTimerManager().ClearTimer(ArenaTimerHandle);
	RoundTime = 0.0f;

	UpdatePlayerRating();
	
	AArenaGameState* ArenaGameState = GetGameState<AArenaGameState>();
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->SetRoundProgress(ERoundProgress::Ended);
		ArenaGameState->SetRemainingTime(RoundTime);
	}

	// TODO :: 결과화면으로 이동 추가
} 

void AArenaGameMode::CheckGameConditions()
{
	if (!HasAuthority()) { return; }

	Super::CheckGameConditions();

	// TODO :: 남은 플레이어 1명일 때 종료
	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (ArenaGameState)
	{
		// TODO :: Get PlayerState RemainPlayer Or Other Conditions
	}

	
}

void AArenaGameMode::UpdateArenaStats()
{
	if (!HasAuthority()) { return; }

	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		float CurrentRemainingTime = ArenaGameState->GetRemainingTime();

		if (CurrentRemainingTime <= 0.0f)
		{
			EndRound();
		}
		else
		{
			ArenaGameState->SetRemainingTime(CurrentRemainingTime - 1.0f);
		}

		// Update Timer
		APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController();

		if (IsValid(MyPlayerController))
		{
			if (ABaseInGameHUD* BaseInGameHUD = Cast<ABaseInGameHUD>(MyPlayerController->GetHUD()))
			{
				if (UBaseInGameWidget* BaseInGameWidget = BaseInGameHUD->GetBaseInGameWidget())
				{
					BaseInGameWidget->UpdateTimerText(ArenaGameState->GetRemainingTime());
				}
			}
		}

		// TODO :: 플레이어체력
		// TODO :: KO 판정

	}
}

void AArenaGameMode::UpdatePlayerRating()
{
}

void AArenaGameMode::UpdateCountdown()
{
	// CountdownText 저장
	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GameState);
	if (IsValid(ArenaGameState))
	{
		ArenaGameState->SetCountdownTime(CountdownTime);

		APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController();

		// 위젯에 보냄
		if (IsValid(MyPlayerController))
		{
			if (AArenaModeHUD* ArenaModeHUD = Cast<AArenaModeHUD>(MyPlayerController->GetHUD()))
			{
				if (UCountdownWidget* CountdownWidget = ArenaModeHUD->GetCountdownWidget())
				{
					// TODO :: 카운트다운 끝나면 안보이게
					if (ArenaGameState->GetCountdownTime() <= 0.0f)
					{
						CountdownWidget->SetCountdownText("START");

						GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
						GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AArenaGameMode::InternalStartRound, 1.0f, false);

						return;
					}
					else
					{
						int32 TimeText = FMath::FloorToInt(ArenaGameState->GetCountdownTime());
						FString FormattedTime = FString::Printf(TEXT("%d"), TimeText);

						CountdownWidget->SetCountdownText(FormattedTime);
					}
				}

				// -1씩 해주고
				CountdownTime -= 1.0f;
				ArenaGameState->SetCountdownTime(CountdownTime);
			}
		}
	}
}

void AArenaGameMode::Server_StartRound_Implementation()
{
	StartRound();
}

bool AArenaGameMode::Server_StartRound_Validate()
{
	return true;
}

void AArenaGameMode::Server_EndRound_Implementation()
{
	EndRound();
}

bool AArenaGameMode::Server_EndRound_Validate()
{
	return true;
}


