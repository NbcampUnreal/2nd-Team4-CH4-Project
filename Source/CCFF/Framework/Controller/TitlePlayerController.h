#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Framework/HUD/TitleHUD.h"
#include "TitlePlayerController.generated.h"

UCLASS()
class CCFF_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void HandleAnyKey(FKey PressedKey);

};
