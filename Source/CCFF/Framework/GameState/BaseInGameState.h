#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseInGameState.generated.h"


UENUM(BlueprintType)
enum class ERoundProgress : uint8
{
	NotStarted UMETA(DisplayName = "Not Started"),
	InProgress UMETA(DisplayName = "In Progress"),
	Ended UMETA(DisplayName = "Ended")
};


UCLASS()
class CCFF_API ABaseInGameState : public AGameState
{
	GENERATED_BODY()

public:
	ABaseInGameState();

	UFUNCTION(BlueprintCallable, Category = "CCFF|GameState")
	virtual void InitializeGameState();

	//UFUNCTION(BlueprintCallable, Category = "CCFF|GameState")
	//virtual float GetRemainingTime() const { return RemainingTime; }

	//UFUNCTION(BlueprintCallable, Category = "CCFF|GameState")
	//virtual void SetRemainingTime(float InTime) { RemainingTime = InTime; }

	//float RemainingTime;

	ERoundProgress RoundProgress;
};
