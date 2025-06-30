// Copyright HigeNeko


#include "Actor/AuraEnemySpawnPoint.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/AuraEnemy.h"

AAuraEnemySpawnPoint::AAuraEnemySpawnPoint()
{
}

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	if (bOverrideHealth)
	{
		UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(Enemy->GetAttributeSet());
		AuraAS->SetMaxHealth(OverrideHealth);
		AuraAS->SetHealth(OverrideHealth);
	}
	Enemy->SpawnDefaultController();
}
