// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleComponent.h"


// Sets default values for this component's properties
UBattleComponent::UBattleComponent()
{
}

float UBattleComponent::GetMeterGainFromDamageTaken(float Damage) const
{
	const float ComboRatio = FMath::Clamp((float)Modifiers.ComboCount / Modifiers.MaxComboBonusThreshold, 0.0f, 1.0f);
	const float Scaling = FMath::Lerp(1.0f, Modifiers.MaxSuperMeterMultiplier, ComboRatio);
	const float SuperMeterGain = Damage * 0.5f * Scaling;
	return SuperMeterGain;
}

FVector UBattleComponent::KnockbackDir(FVector KnockbackAngle, float KnockbackForce, FVector2D DiInput, float DiModifier) const
{
	FVector KnockbackDir = KnockbackAngle.GetSafeNormal();

	FVector2D NormalizedDi = DiInput.GetSafeNormal();
	float Scaling = ComboDiScaling();
	FVector DiAdjustment = FVector(NormalizedDi.X, NormalizedDi.Y, 0.0f) * Scaling * DiModifier;

	FVector FinalKnockback = KnockbackDir * KnockbackForce + DiAdjustment;

	return FinalKnockback;
}

float UBattleComponent::ComboStaleDamage(float Damage, float MinimumDamage) const
{
	if (Modifiers.ComboCount <= 0)
	{
		return Damage;
	}
	float StaleDamage = Damage * FMath::Pow(Modifiers.ComboDamageModifier, Modifiers.ComboCount);
	return FMath::Max(StaleDamage, MinimumDamage);
}

int32 UBattleComponent::ComboStaleHitstun(int32 Hitstun) const
{
	if (Modifiers.ComboCount <= 0)
	{
		return Hitstun;
	}
	float ScaledStun = Hitstun * FMath::Pow(Modifiers.ComboHitStunModifier, Modifiers.ComboCount);
	return FMath::Max(FMath::RoundToInt(ScaledStun), 1);
}

float UBattleComponent::ComboDiScaling() const
{
	if (Modifiers.ComboCount <= 0)
	{
		return Modifiers.DiMultiplier;
	}

	float Ratio = static_cast<float>(Modifiers.ComboCount) / Modifiers.MaxDiBonusThreshold;
	Ratio = FMath::Clamp(Ratio, 0.0f, 1.0f);

	float Scaling = FMath::Lerp(Modifiers.DiMultiplier, Modifiers.MaxDiMultiplier, Ratio);
	return Scaling;
}