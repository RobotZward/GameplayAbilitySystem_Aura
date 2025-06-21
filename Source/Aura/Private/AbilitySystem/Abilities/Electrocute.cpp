// Copyright HigeNeko


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const float Damage = DamageCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ELECTROCUTE</>\n"
			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			// Description
			"<Default>Emits a beam of lightning, "
			"propagating to an additional targets nearby, "
			"connecting with the target, repeatedly causing: </><Damage>%.2f</><Default> lightning damage with a chance to stun</>\n\n"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n"

			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			
			"<Default>Emits a beam of lightning, "
			"propagating to %d additional targets nearby, "
			"connecting with the target, repeatedly causing: </><Damage>%.2f</><Default> lightning damage with a chance to stun</>\n\n"),
			
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShockTargets),
			Damage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n"

			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			
			"<Default>Emits a beam of lightning, "
			"propagating to %d additional targets nearby, "
			"connecting with the target, repeatedly causing: </><Damage>%.2f</><Default> lightning damage with a chance to stun</>\n\n"),
			
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShockTargets),
			Damage);
}
