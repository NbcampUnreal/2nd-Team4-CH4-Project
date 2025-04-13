#include "Framework/UI/RankRowWidget.h"
#include "Components/TextBlock.h"

void URankRowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URankRowWidget::SetHeaderRow()
{
	if (RankText)
	{
		RankText->SetText(FText::FromString(TEXT("Rank")));
	}
	if (NickNameText)
	{
		NickNameText->SetText(FText::FromString(TEXT("NickName")));
	}
	if (TotalDamageText)
	{
		TotalDamageText->SetText(FText::FromString(TEXT("TotalDamage")));
	}
	if (SurvivalTimeText)
	{
		SurvivalTimeText->SetText(FText::FromString(TEXT("SurvivalTime")));
	}
}

void URankRowWidget::SetRankInfo(int32 InRank, const FString& InPlayerName, float InTotalDamage, float InSurvivalTime)
{
	if (RankText)
	{
		RankText->SetText(FText::AsNumber(InRank));
	}
	if (NickNameText)
	{
		NickNameText->SetText(FText::FromString(InPlayerName));
	}
	if (TotalDamageText)
	{
		TotalDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), InTotalDamage)));
	}
	if (SurvivalTimeText)
	{
		SurvivalTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), InSurvivalTime)));
	}
}


