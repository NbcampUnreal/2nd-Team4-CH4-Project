#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Framework/UI/BaseUserWidget.h"
#include "TitleHUD.generated.h"

UCLASS()
class CCFF_API ATitleHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SwitchUI(EOutGameUIState UIState);
	void NotifyAnyKeyPressed();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TitleWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PressStartWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoginWidgetClass;

private:
	UPROPERTY()
	UUserWidget* TitleWidgetInstance = nullptr;

	UPROPERTY()
	UUserWidget* SubWidget = nullptr;

	UFUNCTION()
	void HandleLoginSuccess();
};
