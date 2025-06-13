// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

struct FGameplayAbilitySpec;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FOnAbilityGivenDelegate);
DECLARE_DELEGATE_OneParam(FForEachAbilityDelegate, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityStatusChangedSignature, const FGameplayTag& /*AbilityTag*/ ,const FGameplayTag& /*StatusTag*/);

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
	// 用于在AbilityGiven后进行广播
	FOnAbilityGivenDelegate OnAbilityGivenDelegate;
	// 用于Ability的StatusTag由Locked切换为Eligible并赋予ASC时进行广播
	FAbilityStatusChangedSignature OnAbilityStatusChangedDelegate;

	// Character传入一个StartupAbilities数组，调用该方法来赋予初始能力
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	// Character传入一个StartupPassiveAbilities数组，调用该方法来赋予初始能力并执行
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	// 该函数用于AuraPlayerController中的同名函数，接收GameplayTag并判断激活哪个技能
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	// 该函数用于AuraPlayerController中的同名函数，接收GameplayTag并判断激活哪个技能
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	// 获取bStartupAbilitiesGiven，该变量标识了当前是否已经赋予初始值，Widget Controller会根据此值决定是否还需要绑定委托
	bool GetStartupAbilitiesGiven() const { return bStartupAbilitiesGiven; }
	void ForEachAbility(const FForEachAbilityDelegate& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	
	FGameplayAbilitySpec* GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	void UpdateAbilityStatuses(int32 Level);

protected:
	// 该方法的功能是在FGameplayAbilitySpecContainer ActivatableAbilities变化后将其复制到客户端的ASC，类似OnRep_Health
	// 由于我们将玩家技能初始化的方法放在了ACharacter的PossessedBy方法中，所以只会在服务器调用
	// 综上所述我们需要重写OnRep_ActivateAbilities来让客户端获取ActivatableAbilities
	virtual void OnRep_ActivateAbilities() override;
	// 绑定至OnGameplayEffectAppliedDelegateToSelf并广播FEffectAssetTags代理
	// 由于OnGameplayEffectAppliedDelegateToSelf只会在服务器调用，所以需要使用Client RPC来在服务器调用，在客户端执行
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                         FActiveGameplayEffectHandle ActiveEffectHandle) const;
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);

private:
	// 该变量标识了当前是否已经赋予初始值，Widget Controller会根据此值决定是否还需要绑定委托
	bool bStartupAbilitiesGiven = false;
};
