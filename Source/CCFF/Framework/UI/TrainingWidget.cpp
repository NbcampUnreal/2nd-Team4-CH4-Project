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

	if (ATrainingPlayerController* MyPlayerController = Cast<ATrainingPlayerController>(GetOwningPlayer()))
	{
		MyPlayerController->StartLocalTraining(EnterTime);
	}
}

void UTrainingWidget::OnResetButtonClicked()
{
	if (ATrainingPlayerController* PC = Cast<ATrainingPlayerController>(GetOwningPlayer()))
	{
		PC->EndLocalTraining();
	}
	UpdateTimer(0.f);
	UpdateTrainingStatsData(0.f, 0.f);
}

void UTrainingWidget::UpdateTimer(float CurrentTime)
{
	ATrainingGameState* TGameState = Cast<ATrainingGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!TGameState) return;

	if (TGameState->GetRoundProgress() != ERoundProgress::InProgress)
	{

		// TODO 라운드 업데이트
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
       UE_LOG(LogTemp, Log, TEXT("TotalDamage : %.2f"), TotalDamage);  
   }  

   if (DPSText)  
   {  
       DPSText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), DPS)));  
       UE_LOG(LogTemp, Log, TEXT("DPS : %.2f"), DPS);  
   }  
}

