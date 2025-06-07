// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnBeginOverlap,
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	// 是否在Effect应用后销毁，仅在配置不为Infinite时生效
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	/** 三种持续时间政策的配置 */
	// Instant GE Class
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffectClassArr;
	// 何时激活此Instant GE
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Duration GE Class
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	// 何时激活此Duration GE
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Infinite GE CLass
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	// 何时激活此Infinite GE
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	// 何时移除此Infinite GE
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Applied Effects")
	float ActorLevel = 1.f;

	UFUNCTION()
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	UFUNCTION()
	void RemoveEffectFromTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
};
