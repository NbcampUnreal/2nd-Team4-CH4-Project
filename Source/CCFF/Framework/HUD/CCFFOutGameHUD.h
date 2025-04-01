#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CCFFOutGameHUD.generated.h"

UENUM(BlueprintType)
enum class EOutGameUIState : uint8
{
	EOGUI_TITLE		UMETA(DisplayName = "Title"),
	EOGUI_LOGIN		UMETA(DisplayName = "Login"),
	EOGUI_MAINMENU	UMETA(DisplayName = "MainMenu"),
	EOGUI_MAX		UMETA(Hidden)
};

UCLASS()
class CCFF_API ACCFFOutGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void SwitchUI(EOutGameUIState UIState);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TitleWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoginWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:
	UUserWidget* CurrentWidget;

};
