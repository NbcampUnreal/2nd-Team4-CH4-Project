#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

UCLASS()
class CCFF_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	UFUNCTION(BlueprintCallable)
	void JoinServer(const FString& TargetIP);

	UFUNCTION(BlueprintCallable)
	void FindSessions();

	UFUNCTION()
	void HandleBack();

};
