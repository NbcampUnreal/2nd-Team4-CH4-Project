#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RankRowWidget.generated.h"

class UTextBlock;
UCLASS()
class CCFF_API URankRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ranking")
	void SetHeaderRow();

	UFUNCTION(BlueprintCallable, Category = "Ranking")
	void SetRankInfo(int32 InRank, const FString& InPlayerName, float InTotalDamage, float InSurvivalTime);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RankText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NickNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalDamageText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SurvivalTimeText;
};
