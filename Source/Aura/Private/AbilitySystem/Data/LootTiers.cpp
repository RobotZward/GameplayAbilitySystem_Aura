// Copyright HigeNeko


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems() const
{
	TArray<FLootItem> ReturnItems;

	for (const FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.MaxNumToSpawn; i++)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				ReturnItems.Add(NewItem);
			}
		}
	}

	return ReturnItems;
}
