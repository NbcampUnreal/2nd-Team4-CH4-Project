#pragma once

#include "CoreMinimal.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "ArenaModeHUD.generated.h"

class UTextBlock;
class UCountdownWidget;
class UArenaResultWidget;

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
	void ShowArenaResultWidget();
	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void HideArenaResultWidget();

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void UpdateCountdownText(const FString& InText);

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	FORCEINLINE UCountdownWidget* GetCountdownWidget() const { return CountdownWidget; }
	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	FORCEINLINE UArenaResultWidget* GetArenaResultWidget() const { return ResultWidget; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<UCountdownWidget> CountdownWidgetClass;
	UCountdownWidget* CountdownWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<UArenaResultWidget> ResultWidgetClass;
	UArenaResultWidget* ResultWidget;
};
