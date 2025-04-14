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
	if (KillCountText)
	{
		KillCountText->SetText(FText::FromString(TEXT("KillCount")));
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

void URankRowWidget::SetRankInfo(int32 InRank, const FString& InPlayerName, float InTotalDamage, float InSurvivalTime, int32 InKillCount)
{
	if (RankText)
	{
		RankText->SetText(FText::AsNumber(InRank));
	}
	if (NickNameText)
	{
		NickNameText->SetText(FText::FromString(InPlayerName));
	}
	if (KillCountText)
	{
		KillCountText->SetText(FText::AsNumber(InKillCount));
	}
	if (TotalDamageText)
	{
		TotalDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), InTotalDamage)));
	}
	if (SurvivalTimeText)
	{
		int32 TotalSeconds = FMath::RoundToInt(InSurvivalTime);
		int32 Minutes = TotalSeconds / 60;
		int32 Seconds = TotalSeconds % 60;
		FString TimeString = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);

		SurvivalTimeText->SetText(FText::FromString(TimeString));
	}
}


