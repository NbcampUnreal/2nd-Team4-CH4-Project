#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseInGameHUD.generated.h"

class UBaseInGameWidget;
class UTogglePauseWidget;
class USettingsWidget;

UCLASS()
class CCFF_API ABaseInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void ShowTogglePauseWidget();

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void ShowSettingsWidget();

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void HideSettingsWidget();

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	FORCEINLINE UBaseInGameWidget* GetBaseInGameWidget() const { return BaseInGameWidget; }

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	FORCEINLINE UTogglePauseWidget* GetTogglePauseWidget() const { return TogglePauseWidget; }

	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	FORCEINLINE USettingsWidget* GetSettingsWidget() const { return SettingsWidget; }

	template<typename TWidget>
	TWidget* CreateAndAddWidget(TSubclassOf<TWidget> WidgetClass, int32 ZOrder, ESlateVisibility InitialVisibility = ESlateVisibility::Collapsed)
	{
		TWidget* Widget = nullptr;
		if (WidgetClass)
		{
			Widget = CreateWidget<TWidget>(GetWorld(), WidgetClass);
			if (Widget)
			{
				Widget->AddToViewport(ZOrder);
				Widget->SetVisibility(InitialVisibility);
			}
		}
		return Widget;
	}

protected:
#pragma region BaseInGameWidget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<UBaseInGameWidget> BaseInGameWidgetClass;

	UPROPERTY()
	UBaseInGameWidget* BaseInGameWidget;
#pragma endregion

#pragma region TogglePauseWidget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<UTogglePauseWidget> TogglePauseWidgetClass;

	UPROPERTY()
	UTogglePauseWidget* TogglePauseWidget;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<USettingsWidget> SettingsWidgetClass;

	UPROPERTY()
	USettingsWidget* SettingsWidget;
};
