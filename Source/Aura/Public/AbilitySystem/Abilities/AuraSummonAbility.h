// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	// 获取召唤位置
	UFUNCTION(BlueprintCallable,  Category = "SummonAbility|Summoning")
	TArray<FVector> GetSpawnLocations();
	// 可召唤仆从的数量
	UPROPERTY(EditDefaultsOnly, Category = "SummonAbility|Summoning")
	int32 NumMinions = 5;
	// 可召唤仆从的类型
	UPROPERTY(EditDefaultsOnly, Category = "SummonAbility|Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;
	// 召唤仆从的最小距离
	UPROPERTY(EditDefaultsOnly, Category = "SummonAbility|Summoning")
	float MinSpawnDistance = 50.f;
	// 召唤仆从的最大距离
	UPROPERTY(EditDefaultsOnly, Category = "SummonAbility|Summoning")
	float MaxSpawnDistance = 250.f;
	// 召唤仆从的范围
	UPROPERTY(EditDefaultsOnly, Category = "SummonAbility|Summoning")
	float SpawnSpread = 90.f;

	UFUNCTION(BlueprintPure, Category = "SummonAbility|Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();
};
