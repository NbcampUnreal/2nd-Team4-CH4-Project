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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "CCFF|GameState|HUD")
	virtual void UpdateHUDData();
	UFUNCTION(BlueprintCallable, Category = "CCFF|GameState|HUD")
	virtual void SetRemainingTime(float NewTime);
	UFUNCTION(BlueprintCallable, Category = "CCFF|GameState|HUD")
	virtual void SetRoundProgress(ERoundProgress NewProgress);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CCFF|GameState|HUD")
	float RemainingTime;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CCFF|GameState|HUD")
	ERoundProgress RoundProgress;

};
