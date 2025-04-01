#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CCFFGameState.generated.h"

UCLASS()
class CCFF_API ACCFFGameState : public AGameState
{
	GENERATED_BODY()

public:
	ACCFFGameState();

	UPROPERTY(BlueprintReadOnly, Category = "CCFF|HUD")
	float RemaingTime;

	UPROPERTY(BlueprintReadOnly, Category = "CCFF|HUD")
	int32 ComboCount;

	UPROPERTY(BlueprintReadOnly, Category = "CCFF|Rating")
	float PlayerRating;

	UFUNCTION(BlueprintCallable, Category = "CCFF|HUD")
	void UpdateHUDData();

	UFUNCTION(BlueprintCallable, Category = "CCFF|Round")
	void StoreRoundResult();
};
