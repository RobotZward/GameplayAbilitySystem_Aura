// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

/**
 * 在保存数据时，如果直接使用指针，则会保存其当前的内存地址。下次打开时系统分配的内存很可能会变化，这是未定义的行为。
 * 因此应采用类似的方法，使用结构体数组保存数据。
 */
USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform Transform = FTransform();

	// 存储从Actor中序列化的数据，仅适用于标记为SaveGame的成员变量
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName = FString();

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SavedAbiltiyInfo")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SavedAbiltiyInfo")
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SavedAbiltiyInfo")
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SavedAbiltiyInfo")
	FGameplayTag AbilitySlot = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SavedAbiltiyInfo")
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SavedAbiltiyInfo")
	int32 Level = 1;
};

inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Name");

	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FString MapAssetName = FString("Default Map Asset Name");

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstTimeLoad = true;

	/*
	 * Player
	 */

	// Player Data
	
	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 SpellPoints = 0;

	UPROPERTY()
	int32 AttributePoints = 0;

	// Attribute
	
	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;
	
	UPROPERTY()
	float Resilience = 0.f;
	
	UPROPERTY()
	float Vigor = 0.f;

	/*
	 * Abilities
	 */

	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	/*
	 * Maps & Actors
	 */

	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
};
