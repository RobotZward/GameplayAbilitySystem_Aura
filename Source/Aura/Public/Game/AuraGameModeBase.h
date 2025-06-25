// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo>	CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// 通过指定的Index和SlotName创建存档，会覆盖本地存档
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex) const;
	// 从磁盘中加载指定SlotName的存档，如果没有找到，则创建一个默认ULoadScreenSaveGame并返回
	ULoadScreenSaveGame* LoadSlotData(const FString& SlotName, int32 SlotIndex) const;
	// 从磁盘中删除指定SlotName的存档
	static void DeleteSlot(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	// 根据LoadSlot切换地图
	void TravelToMap(UMVVM_LoadSlot* LoadSlot);
	// 保存数据到UAuraGameInstance
	void SaveDataToGameInstance(UMVVM_LoadSlot* LoadSlot) const;
	// 获取AuraGameInstance中的SlotName和SlotIndex，调用LoadSlotData，从磁盘中读取指定存档
	ULoadScreenSaveGame* RetrieveInGameSaveData() const;
	// 获取AuraGameInstance中的SlotName和SlotIndex，将SaveData保存到磁盘中的指定存档
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject) const;
	// 将Map及其中的Actor保存进SaveData，使用了Archive和Serialize功能
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	// 根据MapName，从SaveData中读取指定Map数据
	void LoadWorldState(UWorld* World) const;

	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	void PlayerDied(ACharacter* DeadCharacter);

protected:
	virtual void BeginPlay() override;
};
