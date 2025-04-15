#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

UCLASS()
class CCFF_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void JoinServer(const FString& InIPAddress);

protected:
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void HandleAnyKey(FKey PressedKey);

};
