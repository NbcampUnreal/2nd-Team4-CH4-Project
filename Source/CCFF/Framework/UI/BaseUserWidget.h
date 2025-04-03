#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

UENUM(BlueprintType)
enum class EOutGameUIState : uint8
{
	EOGUI_TITLE		UMETA(DisplayName = "Title"),
	EOGUI_LOGIN		UMETA(DisplayName = "Login"),
	EOGUI_MAINMENU	UMETA(DisplayName = "MainMenu"),
	EOGUI_MAX UMETA(Hidden)
};

UCLASS()
class CCFF_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RequestUISwitch(EOutGameUIState State);

};
