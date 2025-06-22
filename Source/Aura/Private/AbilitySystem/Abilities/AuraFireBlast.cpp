// Copyright HigeNeko


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const float Damage = DamageCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BLAST</>\n"
			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			// Description
			"<Default>Launches %d </>"
			"<Default>fire balls in all directions, each coming back and "
			"exploding upon return, causing: </>"
			"<Damage>%.2f</><Default> radial fire damage with a chance to burn</>\n\n"),
			// Values
			Level,
			ManaCost,
			Cooldown,
			NumFireBalls,
			Damage);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n"
		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		// Description
		"<Default>Launches %d </>"
		"<Default>fire balls in all directions, each coming back and "
		"exploding upon return, causing: </>"
		"<Damage>%.2f</><Default> radial fire damage with a chance to burn</>\n\n"),
		// Values
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		Damage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnActor = GetAvatarActorFromActorInfo();

		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		
		FireBalls.Add(FireBall);
		
		FireBall->FinishSpawning(SpawnTransform);
	}

	return FireBalls;
}
