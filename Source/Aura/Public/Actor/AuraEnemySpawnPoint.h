// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "AuraEnemySpawnPoint.generated.h"

class AAuraEnemy;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	AAuraEnemySpawnPoint();
	
	void SpawnEnemy();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy Spawn Point|Enemy Class")
	TSubclassOf<AAuraEnemy> EnemyClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy Spawn Point|Enemy Level")
	int32 EnemyLevel = 1 ;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy Spawn Point|Enemy Character Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy Spawn Point|Enemy Health")
	bool bOverrideHealth = false ;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy Spawn Point|Enemy Health")
	float OverrideHealth = 200 ;
};
