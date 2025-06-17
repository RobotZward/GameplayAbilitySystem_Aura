// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
class UAbilityInfo;
class AAuraHUD;
class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(
		const UObject* WorldContextObject,
		FWidgetControllerParams& OutWidgetControllerParams,
		AAuraHUD*& OutAuraHUD
	);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	// 通过传入的ECharacterClass枚举、Level、ASC，从AAuraGameModeBase中获取UCharacterClassInfo，并使用GE初始化Attributes
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	// 用于初始化Enemy的StartupAbilities
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	// 直接返回AAuraGameModeBase中存储的UCharacterClassInfo数据
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	// 直接返回AAuraGameModeBase中存储的UAbilityInfo数据
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	// 根据敌人类别和等级从CharacterClassInfo中获取其奖励经验值
	UFUNCTION()
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level);

	/*
	 * FAuraGameplayEffectContext
	 */
	
	// 该函数用来在蓝图中获取bIsBlickedHit
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中获取bIsCriticalHit
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中获取bIsSuccessfulDebuff
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中获取DebuffDamage
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中获取DebuffFrequency
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中获取DebuffDuration
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中获取DebuffDuration
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中获取DeathImpulse
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中获取KnockbackForce
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);

	// 该函数用来在蓝图中设置bIsBlickedHit
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bIsBlockedHit);

	// 该函数用来在蓝图中设置bIsCriticalHit
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bIsCriticalHit);

	// 该函数用来在蓝图中设置bIsSuccessfulDebuff
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bIsSuccessfulDebuff);

	// 该函数用来在蓝图中设置DebuffDamage
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float DebuffDamage);

	// 该函数用来在蓝图中设置DebuffFrequency
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float DebuffFrequency);

	// 该函数用来在蓝图中设置DebuffDuration
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float DebuffDuration);

	// 该函数用来在蓝图中设置DamageType
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& DamageType);

	// 该函数用来在蓝图中设置DeathImpulse
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& DeathImpulse);

	// 该函数用来在蓝图中设置KnockbackForce
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& KnockbackForce);
	
	// 该函数用来获取一定范围内bDead为false的Character
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(
		const UObject* WorldContextObject,
		UPARAM(ref) TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore,
		float Radius,
		const FVector& SphereOrigin
	);

	// 该函数通过两个Actor的标签来判断是否应该互相伤害
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsFriend(AActor* FirstActor, AActor* SecondActor);

	// 使用自定义的结构体FDamageEffectParams应用GE
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayEffectContextHandle ApplyDamageEffectByDamageEffectParams(const FDamageEffectParams& DamageEffectParams);

	// 根据给定的信息求出一角度范围内的平均旋转
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

	// 根据给定的信息求出一角度范围内的平均向量
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary")
	static TArray<FVector> EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);
};
