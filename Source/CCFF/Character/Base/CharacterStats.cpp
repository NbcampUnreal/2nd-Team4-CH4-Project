#include "CharacterStats.h"

FCharacterStats::FCharacterStats()
	: Gravity(0.0f), GroundFriction(0.0f), AirFriction(0.0f), MaxWalkSpeed(0.0f), MaxRunSpeed(0.0f), MaxFallSpeed(0.0f), MaxAirSpeed(0.0f),
	  MaxSuperMeter(0.0f), SuperMeter(0.0f), MaxBurstMeter(0.0f), BurstMeter(0.0f), BurstMeterGain(0.0f), MaxBlockMeter(0.0f), BlockMeter(0.0f),
	  BlockMeterGain(0.0f), PerfectParryTick(0.0f), DamageTakenModifier(0.0f), DiModifier(0.0f), MaxHealth(0.0f), Health(0.0f)
{
}