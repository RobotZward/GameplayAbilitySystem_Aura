// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/)

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// ActorInfo初始化后的操作，目前用于绑定代理
	void AbilityActorInfoSet();
	// 用于广播GameplayTagContainer的代理
	FEffectAssetTags EffectAssetTags;

	// Character传入一个StartupAbilities数组，调用该方法来赋予初始能力
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// 该函数用于AuraPlayerController中的同名函数，接收GameplayTag并判断激活哪个技能
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	// 该函数用于AuraPlayerController中的同名函数，接收GameplayTag并判断激活哪个技能
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	// 绑定至OnGameplayEffectAppliedDelegateToSelf并广播FEffectAssetTags代理
	// 由于OnGameplayEffectAppliedDelegateToSelf只会在服务器调用，所以需要使用Client RPC来在服务器调用，在客户端执行
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                   FActiveGameplayEffectHandle ActiveEffectHandle) const;
};
