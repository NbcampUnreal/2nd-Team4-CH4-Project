#include "Framework/GameMode/TitleGameMode.h"
#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/HUD/TitleHUD.h"

ATitleGameMode::ATitleGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ATitlePlayerController::StaticClass();
	HUDClass = ATitleHUD::StaticClass();
}
