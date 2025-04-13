#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaResultWidget.generated.h"

class UVerticalBox;
class UButton;
class URankRowWidget;


UCLASS()
class CCFF_API UArenaResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Result")
	void SetRankingInfos(const TArray<FArenaRankInfo>& RankingInfos);

	UFUNCTION(BlueprintCallable, Category = "Result")
	void OnOkButtonClicked();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Result")
	TSubclassOf<URankRowWidget> RowWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* RankingListBox;

	UPROPERTY(meta = (BindWidget))
	UButton* OkButton;
};

