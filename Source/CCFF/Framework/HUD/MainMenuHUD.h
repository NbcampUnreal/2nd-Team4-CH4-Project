#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

UCLASS()
class CCFF_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void ShowMainMenuWidget();

	UFUNCTION(BlueprintCallable)
	void ShowSelectModeWidget();

	UFUNCTION(BlueprintCallable)
	void ShowSettingsWidget();

	UFUNCTION(BlueprintCallable)
	void HideSettingsWidget();

protected:

	template<typename TWidget>
	TWidget* SwitchWidget(TWidget*& CurrentWidget, TSubclassOf<TWidget> WidgetClass, int32 ZOrder = 0);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class USelectModeWidget> SelectModeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class USettingsWidget> SettingsWidgetClass;

private:

	UPROPERTY()
	class UMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	class USelectModeWidget* SelectModeWidget;

	UPROPERTY()
	class USettingsWidget* SettingsWidget;

};

template<typename TWidget>
inline TWidget* AMainMenuHUD::SwitchWidget(TWidget*& CurrentWidget, TSubclassOf<TWidget> WidgetClass, int32 ZOrder)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if (WidgetClass)
	{
		CurrentWidget = CreateWidget<TWidget>(GetWorld(), WidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport(ZOrder);
		}
	}

	return CurrentWidget;
}
