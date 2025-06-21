// Copyright HigeNeko


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const float Damage = DamageCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ARCANE SHARDS</>\n"
			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			// Description
			"<Default>Summon a shard of arcane energy, "
			"causing radial arcane damage of: </><Damage>%.2f</><Default> at the shard origin</>\n\n"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>ARCANE SHARDS</>\n"

			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			
			"<Default>Summon %d shards of arcane energy, "
			"causing radial arcane damage of: </><Damage>%.2f</><Default> at the shard origin</>\n\n"),
			
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShards),
			Damage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n"

			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			
			"<Default>Summon %d shards of arcane energy, "
			"causing radial arcane damage of: </><Damage>%.2f</><Default> at the shard origin</>\n\n"),
			
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShards),
			Damage);
}
