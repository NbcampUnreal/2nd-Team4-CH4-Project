#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ArenaPlayerState.generated.h"


UCLASS()
class CCFF_API AArenaPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AArenaPlayerState();

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE float GetTotalDamage() const { return TotalDamage; }
	FORCEINLINE void SetTotalDamage(float Damage) { TotalDamage = Damage; }

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE float GetSurvivalTime() const { return SurvivalTime; }
	FORCEINLINE void SetSurvivalTime(float Time) { SurvivalTime = Time; }

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE void AddDamage(float Amount) { TotalDamage += Amount; }

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE void AddSurvivalTime(float DeltaTime) { SurvivalTime += DeltaTime; }

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "CCFF|Player Statistics")
	float TotalDamage;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "CCFF|Player Statistics")
	float SurvivalTime;
};
