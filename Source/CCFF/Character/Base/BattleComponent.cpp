// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleComponent.h"


// Sets default values for this component's properties
UBattleComponent::UBattleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBattleComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ABaseCharacter>(GetOwner());
}


// Called every frame
void UBattleComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//적용
void UBattleComponent::ApplyDamage(float Damage, float MinimumDamage) const
{
	float ScaledDamage = ComboStaleDamage(Damage, MinimumDamage);

	// 피격 게이지 회수
	const float ComboRatio = FMath::Clamp((float)ComboCount / MaxComboBonusThreshold, 0.0f, 1.0f);
	const float Scaling = FMath::Lerp(1.0f, MaxSuperMeterMultiplier, ComboRatio);
	const float SuperMeterGain = Damage * 0.5f * Scaling;
	GainSuperMeter(SuperMeterGain);
	GainBurstMeter(BurstGain);
	CachedCharacter->//체력제어
}

void UBattleComponent::ApplyHitstun(int32 Hitstun) const
{
	float ScaledStun = ComboStaleHitstun(Hitstun);
	CachedCharacter->//히트스턴제어
}

void UBattleComponent::ApplyHitLag(int32 Hitlag) const
{
	CachedCharacter->//히트랙제어
}

void UBattleComponent::ApplyBlockStun(int32 BlockStun) const
{
	CachedCharacter->//가드경직제어
}

void UBattleComponent::ApplyKnockback(FVector KnockbackAngle, float KnockbackForce, FVector2D DiInput, float DiModifier) const
{
	FVector KnockbackDir = KnockbackAngle.GetSafeNormal();

	FVector2D NormalizedDi = DiInput.GetSafeNormal();
	float Scaling = CurrentDiScaling();
	FVector DiAdjustment = FVector(NormalizedDi.X, NormalizedDi.Y, 0.0f) * Scaling;

	FVector FinalKnockback = KnockbackDir * KnockbackForce + DiAdjustment;

	CachedCharacter->//넉백제어
}

void UBattleComponent::ApplyGuardMeterDamage(float GuardMeterDamage) const
{
	CachedCharacter->//가드미터데미지제어
}

void UBattleComponent::GuardCrush() const
{
	CachedCharacter->//가드크래쉬
}

//콤보 계산
void UBattleComponent::ResetCombo()
{
	ComboCount = 0;
}

void UBattleComponent::IncreaseCombo()
{
	ComboCount += 1;
}

float UBattleComponent::ComboStaleDamage(float Damage, float MinimumDamage) const
{
	if (ComboCount <= 0)
	{
		return Damage;
	}
	float StaleDamage = Damage * FMath::Pow(ComboDamageModifier, ComboCount);
	return FMath::Max(StaleDamage, MinimumDamage);
}

int32 UBattleComponent::ComboStaleHitstun(int32 Hitstun) const
{
	if (ComboCount <= 0)
	{
		return Hitstun;
	}
	float ScaledStun = Hitstun * FMath::Pow(ComboHitStunModifier, ComboCount);
	return FMath::Max(FMath::RoundToInt(ScaledStun), 1);
}

float UBattleComponent::CurrentDiScaling() const
{
	if (ComboCount <= 0)
	{
		return DiMultiplier;
	}

	float Ratio = static_cast<float>(ComboCount) / MaxDiBonusThreshold;
	Ratio = FMath::Clamp(Ratio, 0.0f, 1.0f);

	float Scaling = FMath::Lerp(DiMultiplier, MaxDiMultiplier, Ratio);
	return Scaling;
}

//전투
void UBattleComponent::OnHit() const
{
	ApplyHitLag(Hitlag);
	GainSuperMeter(Damage);
	//히트 이펙트
	//히트 사운드
}

void UBattleComponent::OnBlock() const
{
	ApplyHitLag(Hitlag);
	GainSuperMeter(GuardDamage);
}

void UBattleComponent::HitData()
{
	if (/*잡기면*/)
	{
		Grabbed();
		return;
	}
	if (/*가드중이면*/)
	{
		Blocked();
		return;
	}
	if (/*버스트면*/)
	{
		Hitted();
		return;
	}
	if (/*투사체 확인*/)
	{
		if (/*투사체 무적이면*/)
		{
			return;
		}
		if (/*투사체 아머면*/)
		{
			ArmorHitted();
			return;
		}
	}
	if (/*무적이면*/)
	{
		return;
	}
	if (/*아머면*/)
	{
		ArmorHitted();
		return;
	}
	Hitted();
	return;
}

void UBattleComponent::Hitted()
{
	if (/*공격동작 도중이면*/)
	{
		Hitlag *= CounterHitStunModifier;
		VictimHitlag *= CounterHitStunModifier;
		Hitstun *= CounterHitStunModifier;
		Damage *= CounterDamageModifier;
	}
	CachedCharacter->//무적관리 열거형 초기화
	CachedCharacter->//State 열거형 Hitted로 변경
	ApplyHitLag(VictimHitlag);
	ApplyHitstun(Hitstun);
	ApplyDamage(Damage, MinimumDamage);
	ApplyKnockback(KnockbackAngle, KnockbackForce, DiInput);
	IncreaseCombo();
	return;
}

void UBattleComponent::ArmorHitted() const
{
	ApplyHitLag(ArmorHitlag);
	ApplyDamage(Damage * ArmorDamageModifier, 0.0f, DiModifier);
	//아머 이펙트
	//아머 사운드
	return;
}

void UBattleComponent::Blocked() const
{
	ApplyBlockStun(BlockStun);
	ApplyKnockback(Direction, GuardPushback, FVector2D (0.0f, 0.0f), 0.0f);
	//가드 이펙트
	//가드 사운드
	return;
}

void UBattleComponent::Grabbed() const
{
	CachedCharacter->//무적관리 열거형 초기화
	CachedCharacter->//State 열거형 Grabbed로 변경
}

void UBattleComponent::Clash() const
{
	ApplyKnockback(KnockbackAngle, KnockbackForce, DIInput);
}

void UBattleComponent::GainSuperMeter(float Amount) const
{
	CachedCharacter->//SuperMeter수정
}

void UBattleComponent::GainBurstMeter(float Amount) const
{
	CachedCharacter->//BurstMeter수정
}