#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CCFFGameMode.generated.h"

UCLASS()
class CCFF_API ACCFFGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ACCFFGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CCFF|Framework")
	void StartRound();

	UFUNCTION(BlueprintCallable, Category = "CCFF|Framework")
	void EndRound();

	UFUNCTION(BlueprintCallable, Category = "CCFF|Framework")
	void CheckGameConditions();

protected:
	FTimerHandle GameTimerHandle;

	UPROPERTY(EditAnywhere, Category = "CCFF|Framework")
	float RoundDuration;

	UPROPERTY(EditAnywhere, Category = "CCFF|Framework")
	int32 MaxHP;

};
