#include "Framework/UI/ArenaResultWidget.h"
#include "Framework/GameState/ArenaGameState.h"
#include "Framework/UI/RankRowWidget.h"
#include "Framework/Controller/CharacterController.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet/GameplayStatics.h"


void UArenaResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OkButton)
	{
		OkButton->OnClicked.AddDynamic(this, &UArenaResultWidget::OnOkButtonClicked);
	}
}

void UArenaResultWidget::SetRankingInfos(const TArray<FArenaRankInfo>& RankingInfos)
{
	if (!IsValid(RankingListBox)) { return; }

	RankingListBox->ClearChildren();

	if (IsValid(RowWidgetClass))
	{
		URankRowWidget* HeaderRow = CreateWidget<URankRowWidget>(GetWorld(), RowWidgetClass);
		if (IsValid(HeaderRow))
		{
			HeaderRow->SetHeaderRow();
			RankingListBox->AddChild(HeaderRow);
		}
	}

	for (const FArenaRankInfo& Info : RankingInfos)
	{
		if (IsValid(RowWidgetClass))
		{
			URankRowWidget* RowWidget = CreateWidget<URankRowWidget>(GetWorld(), RowWidgetClass);
			if (IsValid(RowWidget))
			{
				RowWidget->SetRankInfo(
					Info.Rank,
					Info.PlayerName,
					Info.TotalDamage,
					Info.SurvivalTime,
					Info.KillCount
				);
				RankingListBox->AddChild(RowWidget);
			}
		}
	}
}

void UArenaResultWidget::OnOkButtonClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ACharacterController* MyPlayerController = Cast<ACharacterController>(PC))
		{
			MyPlayerController->ServerReturnToLobby();
		}
	}
}

