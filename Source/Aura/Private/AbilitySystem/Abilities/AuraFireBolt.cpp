// Copyright HigeNeko


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const float Damage = DamageCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n"
			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			// Description
			"<Default>Launched a bolt of fire, "
			"exploding on impact and dealing: </><Damage>%.2f</><Default> fire damage with a chance to burn</>\n\n"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n"

			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			
			"<Default>Launched %d bolts of fire, "
			"exploding on impact and dealing: </><Damage>%.2f</><Default> fire damage with a chance to burn</>\n\n"),
			
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			Damage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>Next Level</>\n"

			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			
			"<Default>Launched %d bolts of fire, "
			"exploding on impact and dealing: </><Damage>%.2f</><Default> fire damage with a chance to burn</>\n\n"),
			
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			Damage);
}

void UAuraFireBolt::SpawnHomingProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	FVector SocketLocation = FVector::ZeroVector;
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	}
	FRotator TargetRotation = (ProjectileTargetLocation - SocketLocation).Rotation(); 
	if (bOverridePitch) TargetRotation.Pitch = PitchOverride;

	const FVector Forward = TargetRotation.Vector();
	const int32 NumFireBolts = 5; // FMath::Min(GetAbilityLevel(), NumProjectiles);
	TArray<FVector> Directions = UAuraAbilitySystemLibrary::EvenlySpacedVectors(Forward, FVector::UpVector, ProjectileSpread, NumFireBolts);
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumFireBolts); 
	
	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		Projectile->FinishSpawning(SpawnTransform);
	}
	
	
}
