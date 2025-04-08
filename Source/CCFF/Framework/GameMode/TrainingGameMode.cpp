#include "Framework/GameMode/TrainingGameMode.h"
#include "Framework/GameState/TrainingGameState.h"
#include "Framework/HUD/TrainingModeHUD.h"
#include "Framework/UI/TrainingWidget.h"
#include "Character/Base/BaseCharacter.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ATrainingGameMode::ATrainingGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MyClassName = "TrainingMode";
	RoundTime = 0.0f;  // Default
}

void ATrainingGameMode::BeginPlay()
{
	Super::BeginPlay();

	RegisterTrainingBotDamageEvents();
}

void ATrainingGameMode::StartTraining()
{
	ATrainingGameState* TGameState = Cast<ATrainingGameState>(GameState);
	if (IsValid(TGameState))
	{
		TGameState->InitializeGameState();
		TGameState->SetRoundProgress(ERoundProgress::InProgress);
		TGameState->SetRemainingTime(RoundTime);
	}	

	GetWorld()->GetTimerManager().SetTimer(TrainingStatusTimerHandle, this, &ATrainingGameMode::UpdateTrainingStats, 1.0f, true);
}

void ATrainingGameMode::EndRound()
{
	Super::EndRound();

	GetWorldTimerManager().ClearTimer(TrainingStatusTimerHandle);
	RoundTime = 0.0f;
	
	if (ATrainingGameState* TGameState = GetGameState<ATrainingGameState>())
	{
		TGameState->SetRoundProgress(ERoundProgress::Ended);
		TGameState->SetRemainingTime(RoundTime);
	}
	

}

void ATrainingGameMode::CheckGameConditions()
{
	Super::CheckGameConditions();
}

void ATrainingGameMode::AddDamage(float DamageAmount)
{
	if (ATrainingGameState* TGameState = GetGameState<ATrainingGameState>())
	{
		float NewDamage = TGameState->GetTotalDamage() + DamageAmount;
		TGameState->SetTotalDamage(NewDamage);
	}
}

void ATrainingGameMode::UpdateTrainingStats()
{
	// Update GameState Data
	ATrainingGameState* TGameState = GetGameState<ATrainingGameState>();
	
	if (IsValid(TGameState))
	{
		// Update RemainingTime
		float CurrentRemainingTime = TGameState->GetRemainingTime();
		if (CurrentRemainingTime > 1.0f)
		{
			float NewRemainingTime = CurrentRemainingTime - 1.0f;
			TGameState->SetRemainingTime(NewRemainingTime);
		}

		// Calculate DPS
		float ElapsedTime = RoundTime - TGameState->GetRemainingTime();
		float NewDPS = (ElapsedTime > 0.0f) ? (TGameState->GetTotalDamage() / ElapsedTime) : 0.0f;
		TGameState->SetDPS(NewDPS);

		// Update HUD
		APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController();

		if (IsValid(MyPlayerController))
		{
			if (ATrainingModeHUD* THUD = Cast<ATrainingModeHUD>(MyPlayerController->GetHUD()))
			{
				if (UTrainingWidget* TWidget = THUD->GetTrainingWidget())
				{
					TWidget->UpdateTimer(TGameState->GetRemainingTime());
					TWidget->UpdateTrainingStatsData(TGameState->GetTotalDamage(), TGameState->GetDPS());
				}
			}
		}

		if (TGameState->GetRemainingTime() <= 0.0f)
		{
			EndRound();
		}
	}
}

void ATrainingGameMode::RegisterTrainingBotDamageEvents()
{
	TArray<AActor*> TrainingBots;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), TrainingBots);
	for (AActor* Bot : TrainingBots)
	{
		if (ABaseCharacter* TrainingBot = Cast<ABaseCharacter>(Bot))
		{
			TrainingBot->OnTakeAnyDamage.AddDynamic(this, &ATrainingGameMode::HandleBotDamage);
		}
	}
}

void ATrainingGameMode::HandleBotDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	AddDamage(Damage);
}

void ATrainingGameMode::SetRoundTime(float InTime)
{
	RoundTime = InTime;

	if (ABaseInGameState* TGameState = GetGameState<ABaseInGameState>())
	{
		TGameState->SetRemainingTime(InTime);
	}
}


