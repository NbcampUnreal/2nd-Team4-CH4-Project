#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCFFBaseUserWidget.generated.h"

class UCCFFGameInstance;
class ACCFFOutgamePlayerController;

UCLASS()
class CCFF_API UCCFFBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "UI")
	ACCFFOutgamePlayerController* GetOutgamePlayerContoller() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UCCFFGameInstance* GetGameInstanceTyped() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RequestUISwitch(EOutGameUIState State);
};
