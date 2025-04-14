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

	UFUNCTION(BlueprintCallable, Category = "CCFF|PlayerStats")
	FORCEINLINE float GetTotalDamage() const { return TotalDamage; }
	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE void SetTotalDamage(float Damage) { TotalDamage = Damage; }

	UFUNCTION(BlueprintCallable, Category = "CCFF|PlayerStats")
	FORCEINLINE float GetSurvivalTime() const { return SurvivalTime; }
	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE void SetSurvivalTime(float Time) { SurvivalTime = Time; }
	
	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE FString GetPlayerNickname() const { return PlayerNickname; }
	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE void SetPlayerNickname(const FString& InNickname) { PlayerNickname = InNickname; }
	
	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE int32 GetKillCount() const { return KillCount; }
	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	FORCEINLINE void SetKillCount(int Kill) { KillCount = Kill; }

	UFUNCTION(BlueprintCallable, Category = "CCFF|PlayerStats")
	FORCEINLINE void AddDamage(float Amount) { TotalDamage += Amount; }

	UFUNCTION(BlueprintCallable, Category = "CCFF|PlayerStats")
	FORCEINLINE void AddSurvivalTime(float DeltaTime) { SurvivalTime += DeltaTime; }


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "CCFF|PlayerStats")
	float TotalDamage;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "CCFF|PlayerStats")
	float SurvivalTime;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "CCFF|Player Statistics")
	FString PlayerNickname;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "CCFF|PlayerStats")
	int32 MaxLives;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "CCFF|PlayerStats")
	int32 KillCount;
};
