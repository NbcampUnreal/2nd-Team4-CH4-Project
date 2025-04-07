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

	UFUNCTION()
	void ShowMainMenuWidget();
	
	UFUNCTION()
	void ShowSelectModeWidget();
	
	UFUNCTION()
	void ShowArenaWidget();
	
	UFUNCTION()
	void ShowSettingsWidget();
	
	UFUNCTION()
	void HideSettingsWidget();

	UFUNCTION()
	void ReturnToPreviousWidget();

	UFUNCTION()
	void ShowLoadingWidget();

	UFUNCTION()
	void HideLoadingWidget();

protected:
	void PushWidget(UUserWidget* WidgetClass);
	void PopWidget();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USelectModeWidget> SelectModeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UArenaModeWidget> ArenaModeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USettingsWidget> SettingsWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	class UMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	class USelectModeWidget* SelectModeWidget;

	UPROPERTY()
	class UArenaModeWidget* ArenaModeWidget;

	UPROPERTY()
	class USettingsWidget* SettingsWidget;

	UPROPERTY()
	class UUserWidget* LoadingWidget;

	UPROPERTY()
	TArray<UUserWidget*> WidgetStack;

};
