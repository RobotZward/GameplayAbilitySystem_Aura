// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffChance = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDamage = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffFrequency = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDuration = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|DeathImpulse")
    float DeathImpulseMagnitude = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Knockback")
	float KnockbackForceMagnitude = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Knockback")
	float KnockbackChance = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Radial")
	bool bIsRadialDamage = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Radial")
	float RadialDamageInnerRadius = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Radial")
	float RadialDamageOuterRadius = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Radial")
	FVector RadialDamageOrigin = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat DamageCurve;
};
