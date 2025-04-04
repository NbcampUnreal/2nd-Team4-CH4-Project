#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseInGameHUD.generated.h"

class UBaseInGameWidget;

UCLASS()
class CCFF_API ABaseInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|HUD")
	TSubclassOf<UBaseInGameWidget> BaseInGameHUDWidgetClass;

	UPROPERTY()
	UBaseInGameWidget* BaseInGameHUDWidget;
};
