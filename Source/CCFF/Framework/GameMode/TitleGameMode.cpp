#include "Framework/GameMode/TitleGameMode.h"
#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/HUD/TitleHUD.h"

ATitleGameMode::ATitleGameMode()
{
	PlayerControllerClass = ATitlePlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	HUDClass = ATitleHUD::StaticClass();
}
