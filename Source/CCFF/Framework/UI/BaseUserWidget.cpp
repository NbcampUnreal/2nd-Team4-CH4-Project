#include "Framework/UI/BaseUserWidget.h"
#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/HUD/TitleHUD.h"

void UBaseUserWidget::RequestUISwitch(EOutGameUIState State)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ATitleHUD* HUD = Cast<ATitleHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			HUD->SwitchUI(State);
		}
	}
}
