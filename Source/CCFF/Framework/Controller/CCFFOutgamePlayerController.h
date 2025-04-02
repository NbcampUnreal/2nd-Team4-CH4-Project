#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Framework/HUD/CCFFOutGameHUD.h"
#include "CCFFOutgamePlayerController.generated.h"

UCLASS()
class CCFF_API ACCFFOutgamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void HandleAnyKey();

public:
	void RequestUIChange(EOutGameUIState NextState);

};
