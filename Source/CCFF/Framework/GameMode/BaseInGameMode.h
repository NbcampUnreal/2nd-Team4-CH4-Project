#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseInGameMode.generated.h"

UCLASS()
class CCFF_API ABaseInGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABaseInGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CCFF|GameMode|Round")
	virtual void StartRound();

	UFUNCTION(BlueprintCallable, Category = "CCFF|GameMode|Round")
	virtual void EndRound();

	UFUNCTION(BlueprintCallable, Category = "CCFF|GameMode|Round")
	virtual void CheckGameConditions();

	UPROPERTY()
	float RoundTime;

protected:
	UFUNCTION()
	virtual void ResetSubsystem();

	FTimerHandle GameTimerHandle;
	FString MyClassName;
	
};
