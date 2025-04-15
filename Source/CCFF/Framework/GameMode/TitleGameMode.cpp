#include "Framework/GameMode/TitleGameMode.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/HUD/TitleHUD.h"

ATitleGameMode::ATitleGameMode()
{
	PlayerControllerClass = ATitlePlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	HUDClass = ATitleHUD::StaticClass();
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UCCFFGameInstance* CCFFGameInstance = GetGameInstance<UCCFFGameInstance>())
	{
		CCFFGameInstance->PlayBGMForCurrentMap();
	}
}
