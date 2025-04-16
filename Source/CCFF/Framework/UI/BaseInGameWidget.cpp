#include "Framework/UI/BaseInGameWidget.h"

#include "Character/Base/BaseCharacter.h"
#include "Framework/UI/Character/ProfileWidget.h"
#include "Framework/UI/Character/SideBarWidget.h"
#include "Character/Components/StatusComponent.h"
#include "Components/TextBlock.h"


class ABaseCharacter;

void UBaseInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC=Cast<APlayerController>(GetOwningPlayer()))
	{
		if (PC->IsLocalController())
		{
			if (ABaseCharacter* MyCharacter=Cast<ABaseCharacter>(PC->GetPawn()))
			{
				MyCharacter->SetHUDWidget(this);
			}	
		}
	}
}

void UBaseInGameWidget::UpdateTimerText_Implementation(const float NewTime)
{
	int32 Minutes = FMath::FloorToInt(NewTime / 60.0f);
	int32 Seconds = FMath::FloorToInt(NewTime) % 60;
	FString FormattedTime = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);

	if (TimerText)
	{
		TimerText->SetText(FText::FromString(FormattedTime));
	}
}

void UBaseInGameWidget::UpdateHealthBar(const float InPercentage)
{
	if (ProfileWidget)
	{
		ProfileWidget->UpdateHealthBar(InPercentage);
		//UE_LOG(LogTemp,Log,TEXT("OnRep_CurrentHP: %0.1f"),InPercentage);
	}
}

void UBaseInGameWidget::UpdateSuperMeterBar(const float InPercentage)
{
	if (SideBarWidget)
	{
		SideBarWidget->UpdateSuperMeterBar(InPercentage);
	}
}

void UBaseInGameWidget::UpdateBurstMeterBar(const float InPercentage)
{
	if (SideBarWidget)
	{
		SideBarWidget->UpdateBurstMeterBar(InPercentage);
	}
}

void UBaseInGameWidget::InitializeHUDWidget(UStatusComponent* InStatusComponent)
{
	float Percentage=FMath::Clamp(InStatusComponent->GetCurrentHP()/InStatusComponent->GetMaxHP(), 0, InStatusComponent->GetMaxHP());
	UpdateHealthBar(Percentage);

	Percentage=FMath::Clamp(InStatusComponent->GetBurstMeter()/InStatusComponent->GetMaxBurstMeter(), 0, InStatusComponent->GetMaxBurstMeter());
	UpdateBurstMeterBar(Percentage);

	Percentage=FMath::Clamp(InStatusComponent->GetSuperMeter()/InStatusComponent->GetMaxSuperMeter(), 0, InStatusComponent->GetMaxSuperMeter());
	UpdateSuperMeterBar(Percentage);
}

