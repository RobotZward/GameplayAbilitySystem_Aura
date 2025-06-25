// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Loot Tires|Spawning")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditAnywhere, Category = "Loot Tires|Spawning")
	float ChanceToSpawn = 0.f;

	UPROPERTY(EditAnywhere, Category = "Loot Tires|Spawning")
	int32 MaxNumToSpawn = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Loot Tires|Spawning")
	bool bLootLevelOverride = true;
};

/**
 * 
 */
UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loot Tires|Spawning")
	TArray<FLootItem> LootItems;
};
