#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseInGameMode.generated.h"

UCLASS()
class CCFF_API ABaseInGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABaseInGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CCFF|GameMode|Round")
	virtual void StartRound();

	UFUNCTION(BlueprintCallable, Category = "CCFF|GameMode|Round")
	virtual void EndRound();

	UFUNCTION(BlueprintCallable, Category = "CCFF|GameMode|Round")
	virtual void CheckGameConditions();

protected:
	UPROPERTY(EditAnywhere, Category = "CCFF|GameMode|Round")
	float RoundDuration;

	FTimerHandle GameTimerHandle;
	FString MyClassName;

private:
	void ResetSubsystem();
};
