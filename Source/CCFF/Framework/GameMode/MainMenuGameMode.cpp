#include "Framework/GameMode/MainMenuGameMode.h"
#include "Framework/GameInstance/CCFFGameInstance.h"

void AMainMenuGameMode::BeginPlay()
{
	if (UCCFFGameInstance* CCFFGameInstance = GetGameInstance<UCCFFGameInstance>())
	{
		CCFFGameInstance->PlayBGMForCurrentMap();
	}
}
