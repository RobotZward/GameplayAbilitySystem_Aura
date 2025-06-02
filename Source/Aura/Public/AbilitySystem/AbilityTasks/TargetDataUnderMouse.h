// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, Data);

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;
	// 我们在这个函数中定义FGameplayAbilityTargetData并将其发送至服务器
	void SendMouseCursorData();
	// 服务器会在Activate中将该函数绑定至AbilityTargetDataSetDelegate，以便在TargetData发送过来时接收广播
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
