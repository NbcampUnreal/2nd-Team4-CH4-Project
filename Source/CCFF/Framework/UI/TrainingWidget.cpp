#include "Framework/UI/TrainingWidget.h"
#include "Framework/GameState/TrainingGameState.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Controller/TrainingPlayerController.h"

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
	if (!TimeInputBox) return;
	float EnterTime = FCString::Atof(*TimeInputBox->GetText().ToString());
	EnterTime = FMath::Max(0.f, EnterTime);

	if (ATrainingPlayerController* TrainingPlayerController = Cast<ATrainingPlayerController>(GetOwningPlayer()))
	{
		TrainingPlayerController->StartLocalTraining(EnterTime);
	}
}

void UTrainingWidget::OnResetButtonClicked()
{
	if (ATrainingPlayerController* TrainingPlayerController = Cast<ATrainingPlayerController>(GetOwningPlayer()))
	{
		TrainingPlayerController->EndLocalTraining();
	}

	UpdateTimer(0.f);
	UpdateTrainingStatsData(0.f, 0.f);
}

void UTrainingWidget::UpdateTimer(float CurrentTime)
{
	ATrainingGameState* TrainingGameState = Cast<ATrainingGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!IsValid(TrainingGameState)) return;

	if (TrainingGameState->GetRoundProgress() != ERoundProgress::InProgress)
	{
		CurrentTime = 0.0f;
	}

	if (APlayerController* MyPlayerController = GetOwningPlayer())
	{
		if (ABaseInGameHUD* BaseInGameHUD = Cast<ABaseInGameHUD>(MyPlayerController->GetHUD()))
		{
			if (UBaseInGameWidget* BaseInGameWidget = BaseInGameHUD->GetBaseInGameWidget())
			{
				BaseInGameWidget->UpdateTimerText(CurrentTime);
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

