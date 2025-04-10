#pragma once

#include "CoreMinimal.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "ArenaModeHUD.generated.h"

class UTextBlock;
class UCountdownWidget;

UCLASS()
class CCFF_API AArenaModeHUD : public ABaseInGameHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void ShowCountdownWidget();
	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void HideCountdownWidget();

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	FORCEINLINE UCountdownWidget* GetCountdownWidget() const { return CountdownWidget; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<UCountdownWidget> CountdownWidgetClass;
	UCountdownWidget* CountdownWidget;
};
