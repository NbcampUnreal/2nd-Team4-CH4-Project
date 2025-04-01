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

//����
void UBattleComponent::ApplyDamage(float Damage, float MinimumDamage) const
{
	float ScaledDamage = ComboStaleDamage(Damage, MinimumDamage);

	// �ǰ� ������ ȸ��
	const float ComboRatio = FMath::Clamp((float)ComboCount / MaxComboBonusThreshold, 0.0f, 1.0f);
	const float Scaling = FMath::Lerp(1.0f, MaxSuperMeterMultiplier, ComboRatio);
	const float SuperMeterGain = Damage * 0.5f * Scaling;
	GainSuperMeter(SuperMeterGain);
	GainBurstMeter(BurstGain);
	CachedCharacter->//ü������
}

void UBattleComponent::ApplyHitstun(int32 Hitstun) const
{
	float ScaledStun = ComboStaleHitstun(Hitstun);
	CachedCharacter->//��Ʈ��������
}

void UBattleComponent::ApplyHitLag(int32 Hitlag) const
{
	CachedCharacter->//��Ʈ������
}

void UBattleComponent::ApplyBlockStun(int32 BlockStun) const
{
	CachedCharacter->//�����������
}

void UBattleComponent::ApplyKnockback(FVector KnockbackAngle, float KnockbackForce, FVector2D DiInput, float DiModifier) const
{
	FVector KnockbackDir = KnockbackAngle.GetSafeNormal();

	FVector2D NormalizedDi = DiInput.GetSafeNormal();
	float Scaling = CurrentDiScaling();
	FVector DiAdjustment = FVector(NormalizedDi.X, NormalizedDi.Y, 0.0f) * Scaling;

	FVector FinalKnockback = KnockbackDir * KnockbackForce + DiAdjustment;

	CachedCharacter->//�˹�����
}

void UBattleComponent::ApplyGuardMeterDamage(float GuardMeterDamage) const
{
	CachedCharacter->//������͵���������
}

void UBattleComponent::GuardCrush() const
{
	CachedCharacter->//����ũ����
}

//�޺� ���
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

//����
void UBattleComponent::OnHit() const
{
	ApplyHitLag(Hitlag);
	GainSuperMeter(Damage);
	//��Ʈ ����Ʈ
	//��Ʈ ����
}

void UBattleComponent::OnBlock() const
{
	ApplyHitLag(Hitlag);
	GainSuperMeter(GuardDamage);
}

void UBattleComponent::HitData()
{
	if (/*����*/)
	{
		Grabbed();
		return;
	}
	if (/*�������̸�*/)
	{
		Blocked();
		return;
	}
	if (/*����Ʈ��*/)
	{
		Hitted();
		return;
	}
	if (/*����ü Ȯ��*/)
	{
		if (/*����ü �����̸�*/)
		{
			return;
		}
		if (/*����ü �ƸӸ�*/)
		{
			ArmorHitted();
			return;
		}
	}
	if (/*�����̸�*/)
	{
		return;
	}
	if (/*�ƸӸ�*/)
	{
		ArmorHitted();
		return;
	}
	Hitted();
	return;
}

void UBattleComponent::Hitted()
{
	if (/*���ݵ��� �����̸�*/)
	{
		Hitlag *= CounterHitStunModifier;
		VictimHitlag *= CounterHitStunModifier;
		Hitstun *= CounterHitStunModifier;
		Damage *= CounterDamageModifier;
	}
	CachedCharacter->//�������� ������ �ʱ�ȭ
	CachedCharacter->//State ������ Hitted�� ����
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
	//�Ƹ� ����Ʈ
	//�Ƹ� ����
	return;
}

void UBattleComponent::Blocked() const
{
	ApplyBlockStun(BlockStun);
	ApplyKnockback(Direction, GuardPushback, FVector2D (0.0f, 0.0f), 0.0f);
	//���� ����Ʈ
	//���� ����
	return;
}

void UBattleComponent::Grabbed() const
{
	CachedCharacter->//�������� ������ �ʱ�ȭ
	CachedCharacter->//State ������ Grabbed�� ����
}

void UBattleComponent::Clash() const
{
	ApplyKnockback(KnockbackAngle, KnockbackForce, DIInput);
}

void UBattleComponent::GainSuperMeter(float Amount) const
{
	CachedCharacter->//SuperMeter����
}

void UBattleComponent::GainBurstMeter(float Amount) const
{
	CachedCharacter->//BurstMeter����
}