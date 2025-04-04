#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
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
	TWidget* SwitchWidget(TWidget* TargetWidget, TSubclassOf<TWidget> WidgetClass, int32 ZOrder = 0);

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

	UPROPERTY()
	UUserWidget* LastActiveWidget;

};

template<typename TWidget>
inline TWidget* AMainMenuHUD::SwitchWidget(TWidget* TargetWidget, TSubclassOf<TWidget> WidgetClass, int32 ZOrder)
{
	if (LastActiveWidget)
	{
		LastActiveWidget->RemoveFromParent();
		LastActiveWidget = nullptr;
	}

	if (WidgetClass)
	{
		TargetWidget = CreateWidget<TWidget>(GetWorld(), WidgetClass);
		if (TargetWidget)
		{
			TargetWidget->AddToViewport(ZOrder);
			LastActiveWidget = TargetWidget;
		}
	}

	return TargetWidget;
}
