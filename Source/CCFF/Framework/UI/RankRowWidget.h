#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "RankRowWidget.generated.h"

class UTextBlock;
UCLASS()
class CCFF_API URankRowWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CCFF|Ranking")
	void SetHeaderRow();

	UFUNCTION(BlueprintCallable, Category = "CCFF|Ranking")
	void SetRankInfo(
		int32 InRank,
		const FString& InPlayerName,
		float InTotalDamage,
		float InSurvivalTime,
		int32 InKillCount
	);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RankText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NickNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalDamageText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SurvivalTimeText;
};
