#include "Framework/GameMode/CCFFTitleGameMode.h"
#include "Framework/Controller/CCFFOutgamePlayerController.h"
#include "Framework/HUD/CCFFOutGameHUD.h"

ACCFFTitleGameMode::ACCFFTitleGameMode()
{
	PlayerControllerClass = ACCFFOutgamePlayerController::StaticClass();
	HUDClass = ACCFFOutGameHUD::StaticClass();
}
