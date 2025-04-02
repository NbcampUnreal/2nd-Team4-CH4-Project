// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseCharacter.h"
#include "BattleComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CCFF_API UBattleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBattleComponent();

	UPROPERTY(BlueprintReadOnly)
	int32 ComboCount = 0;

private:
	TWeakObjectPtr<ABaseCharacter> CachedCharacter;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	float ComboDamageModifier = 0.8f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	float ComboHitStunModifier = 0.8f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	float ComboDiModifier = 1.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	float DiMultiplier = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	int32 MaxDiBonusThreshold = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	int32 MaxDiMultiplier = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Armor")
	int32 ArmorHitlag = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Armor")
	float ArmorDamageModifier = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Counter")
	float CounterDamageModifier = 1.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Counter")
	float CounterHitStunModifier = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meter")
	int32 MaxComboBonusThreshold = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meter")
	float MaxSuperMeterMultiplier = 4.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meter")
	float BurstGain = 5.0f;

	// 적용
	void ApplyDamage(float Damage, float MinimumDamage) const;
	void ApplyHitstun(int32 Hitstun) const;
	void ApplyHitLag(int32 Hitlag) const;
	void ApplyBlockStun(int32 BlockStun) const;
	void ApplyKnockback(FVector KnockbackAngle, float KnockbackForce, FVector2D DIInput, float DiModifier) const;
	void ApplyGuardMeterDamage(float GuardMeterDamage) const;
	void GuardCrush() const;

	// 콤보 계산
	void ResetCombo();
	void IncreaseCombo();
	float ComboStaleDamage(float Damage, float MinimumDamage) const;
	int32 ComboStaleHitstun(int32 Hitstun) const;
	float CurrentDiScaling() const;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// 전투
	UFUNCTION(BlueprintCallable)
	void OnHit() const;
	UFUNCTION(BlueprintCallable)
	void OnBlock() const;
	UFUNCTION(BlueprintCallable)
	void HitData();
	UFUNCTION(BlueprintCallable)
	void Hitted();
	UFUNCTION(BlueprintCallable)
	void ArmorHitted() const;
	UFUNCTION(BlueprintCallable)
	void Blocked() const;
	UFUNCTION(BlueprintCallable)
	void Grabbed() const;
	UFUNCTION(BlueprintCallable)
	void Clash() const;

	// 미터
	void GainSuperMeter(float Amount) const;
	void GainBurstMeter(float Amount) const;
};
