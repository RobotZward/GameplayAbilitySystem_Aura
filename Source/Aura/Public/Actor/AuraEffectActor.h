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
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	/*
	 * Spawn Animation
	 */
	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;
	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup Movement")
	bool bRotate = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup Movement")
	float RotationRate = 45.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup Movement")
	bool bSinusoidalMovement = false;
	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();
	UFUNCTION(BlueprintCallable)
	void StartRotation();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup Movement")
	float SineAmplitude = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup Movement")
	float SinePeriodConstant = 1.f;	// 2 * PI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup Movement")
	FVector InitialLocation = FVector();
	/*
	 * End Spawn Animation
	 */
	

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Applied Effects")
	float ActorLevel = 1.f;

	UFUNCTION()
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	UFUNCTION()
	void RemoveEffectFromTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

private:
	float RunningTime = 0.f;
	void ItemMovement(float DeltaSeconds);
};
