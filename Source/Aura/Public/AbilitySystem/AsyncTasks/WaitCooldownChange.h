// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayEffectSpec;
struct FGameplayTag;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangedSignature, float, TimeRemaining);

/**
 * 
 */
// NOTE: 可以跳过meta=(ExposedAsyncProxy = "AsyncTask")将自身作为节点的输出引脚暴露给蓝图
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangedSignature OnCooldownStart;
	UPROPERTY(BlueprintAssignable)
	FCooldownChangedSignature OnCooldownEnd;
	UFUNCTION(BlueprintCallable, Category = "AuraTask|WaitCooldownChange", meta = (DisplayName = "WaitCooldownChange", BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* CreateWaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable, Category = "AuraTask|WaitCooldownChange")
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
};
