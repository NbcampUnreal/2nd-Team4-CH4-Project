#include "Framework/HUD/BaseInGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Framework/UI/BaseInGameWidget.h"


void ABaseInGameHUD::BeginPlay()
{
	if (IsValid(BaseInGameHUDWidgetClass))
	{
		BaseInGameHUDWidget = CreateWidget<UBaseInGameWidget>(GetWorld(), BaseInGameHUDWidgetClass);
		
		if (IsValid(BaseInGameHUDWidget))
			BaseInGameHUDWidget->AddToViewport(0);
	}
}
