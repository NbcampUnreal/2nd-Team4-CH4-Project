#include "Framework/UI/CCFFBaseUserWidget.h"
#include "Framework/Controller/CCFFOutgamePlayerController.h"
#include "Framework/GameInstance/CCFFGameInstance.h"

ACCFFOutgamePlayerController* UCCFFBaseUserWidget::GetOutgamePlayerContoller() const
{
	return Cast<ACCFFOutgamePlayerController>(GetOwningPlayer());
}

UCCFFGameInstance* UCCFFBaseUserWidget::GetGameInstanceTyped() const
{
	return Cast<UCCFFGameInstance>(GetGameInstance());
}

void UCCFFBaseUserWidget::RequestUISwitch(EOutGameUIState State)
{
	if (ACCFFOutgamePlayerController* PlayerController = GetOutgamePlayerContoller())
	{
		PlayerController->RequestUIChange(State);
	}
}
