// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UAnimMontage;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegisteredSignature, UAbilitySystemComponent*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamageSignature, float /*DamageAmount*/);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage;
	// 用于区分同一Actor的不同蒙太奇，用于send/wait gameplay event 以及在GC中查找数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	// 用于确定攻击检测的触发点或发射物的生成点的类型，获取位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateFacingTarget(const FVector& FacingTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetHitReactMontage();

	virtual void Die(const FVector& DeathImpulse) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetAvatar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FTaggedMontage GetRandomAttackMontage(bool& bSuccess);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetMinionCount();
	// 增加随从计数，仅限正值
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IncrementMinionCount(int32 Amount);
	// 减少随从计数，仅限正值
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DecrementMinionCount(int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetInShockLoop(bool bInLoop);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMeshComponent* GetWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsBeingShocked() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsBeingShocked(bool bInShocked);

	virtual FOnASCRegisteredSignature& GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeathSignature& GetOnDeathSignature() = 0;
	virtual FOnTakeDamageSignature& GetOnTakeDamageSignature() = 0;
};
