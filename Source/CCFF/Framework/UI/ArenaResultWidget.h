#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "ArenaResultWidget.generated.h"

class UVerticalBox;
class UButton;
class URankRowWidget;


UCLASS()
class CCFF_API UArenaResultWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CCFF|Result")
	void SetRankingInfos(const TArray<FArenaRankInfo>& RankingInfos);

	UFUNCTION(BlueprintCallable, Category = "CCFF|Result")
	void OnOkButtonClicked();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|Result")
	TSubclassOf<URankRowWidget> RowWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* RankingListBox;

	UPROPERTY(meta = (BindWidget))
	UButton* OkButton;
};

