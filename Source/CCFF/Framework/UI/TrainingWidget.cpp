#include "Framework/UI/TrainingWidget.h"
#include "Framework/GameMode/TrainingGameMode.h"
#include "Framework/GameState/TrainingGameState.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


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

	UpdateTimer(0.0f);
}


void UTrainingWidget::OnStartButtonClicked()
{
	if (IsValid(TimeInputBox))
	{
		// Get TimeInputBox
		const FString InputStr = TimeInputBox->GetText().ToString();
		float EnterTime = FCString::Atof(*InputStr);
		if (EnterTime < 0.f)
			EnterTime = 0.f;

		// Set Data in GameMode
		if (ATrainingGameMode* TGameMode = Cast<ATrainingGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			TGameMode->SetRoundTime(EnterTime);
			TGameMode->StartTraining();
		}
	}
}

void UTrainingWidget::OnResetButtonClicked()
{
	ATrainingGameMode* TGameMode = Cast<ATrainingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	ATrainingGameState* TGameState = Cast<ATrainingGameState>(UGameplayStatics::GetGameState(GetWorld()));


	if (IsValid(TGameMode) && IsValid(TGameState))
	{
		TGameMode->EndRound();
		TGameState->InitializeGameState();
		UpdateTimer(TGameState->GetRemainingTime());
		UpdateTrainingStatsData(TGameState->GetTotalDamage(), TGameState->GetDPS());
	}
}

void UTrainingWidget::UpdateTimer(float CurrentTime)
{
	ATrainingGameState* TGameState = Cast<ATrainingGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!TGameState) return;

	if (TGameState->GetRoundProgress() != ERoundProgress::InProgress)
	{
		CurrentTime = 0.0f;
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ABaseInGameHUD* HUD = Cast<ABaseInGameHUD>(PC->GetHUD()))
		{
			if (UBaseInGameWidget* BaseWidget = HUD->GetBaseInGameWidget())
			{
				BaseWidget->UpdateTimerText(CurrentTime);
				return;
			}
		}
	}
}

void UTrainingWidget::UpdateTrainingStatsData(float TotalDamage, float DPS)
{
	if (TotalDamageText)
	{
		TotalDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), TotalDamage)));
	}

	if (DPSText)
	{
		DPSText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), DPS)));
	}
}

