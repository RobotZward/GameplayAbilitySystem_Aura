// Copyright HigeNeko


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

// 使用经典C++定义方式，不需要反射
struct AuraDamageStatics
{
	// 采用宏进行声明
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	// 这个Map用来保存Tag和Definition的对应关系，以便在循环中根据Tag进行捕获
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	AuraDamageStatics()
	{
		// 采用宏进行定义分别需要传入：UAttributeSet、AttributeName、CaptureSourceType、bSnapShot
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
	}
	// 由于静态变量初始化顺序不固定，所以放在构造函数中可能存在初始化时Tag还未初始化
	// 因此采用懒加载的方式初始化，即将初始化方法放在Get函数中，等到获取时再第一次初始化
	void InitTagsToCaptureDefs()
	{
		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
	}
};

// 采用静态函数和静态变量，每次获取的均为同一变量
static const AuraDamageStatics& GetAuraDamageStatics()
{
	static AuraDamageStatics DStatics;
	DStatics.InitTagsToCaptureDefs();
	return DStatics;
}

// 提供一种安全获取SetByCaller的方法，通过直接获取FGameplayEffectSpec中的TMap：SetByCallerTagMagnitudes并查询指定标签来实现 
float GetSafeSetByCaller(const FGameplayEffectSpec& Spec, const FGameplayTag& Tag, float DefaultValue = 0.f, bool bWarningIfNotFound = true )
{
	if (Spec.SetByCallerTagMagnitudes.Find(Tag))
	{
		return Spec.GetSetByCallerMagnitude(Tag, bWarningIfNotFound, DefaultValue);
	}

	if (bWarningIfNotFound)
	{
		UE_LOG(LogAura, Warning, TEXT("SetByCaller tag [%s] not found, using default value [%f]"), *Tag.ToString(), DefaultValue);
	}
	return DefaultValue;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// 将Definition添加到捕获列表
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvaluationParameters, const FGameplayEffectSpec& Spec) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	for (TPair<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = GetSafeSetByCaller(Spec, Pair.Key, -1.f, false);
		if (TypeDamage > -0.5f)
		{
			const float SourceDebuffChance = GetSafeSetByCaller(Spec, GameplayTags.Debuff_Chance, -1.f, false);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().TagsToCaptureDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(0, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, bDebuff);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = GetSafeSetByCaller(Spec, GameplayTags.Debuff_Damage, -1.f, false);
				const float DebuffFrequency = GetSafeSetByCaller(Spec, GameplayTags.Debuff_Frequency, -1.f, false);
				const float DebuffDuration = GetSafeSetByCaller(Spec, GameplayTags.Debuff_Duration, -1.f, false);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// FGameplayEffectCustomExecutionParameters中可以获取到很多信息
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	FAggregatorEvaluateParameters EvaluationParameters;
	AActor* SourceAvatarActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatarActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatarActor->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatarActor);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatarActor->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatarActor);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// 构建FAggregatorEvaluateParameters
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Debuff
	DetermineDebuff(ExecutionParams, EvaluationParameters, Spec);

	/*
	 * Base damage and block chance.
	 */

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageType = Pair.Key;
		const FGameplayTag ResistanceType = Pair.Value;

		// const auto& Map1 = GetAuraDamageStatics().TagsToCaptureDefs;
		// const auto Map2 = AuraDamageStatics().TagsToCaptureDefs;
		
		checkf(GetAuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceType),
		       TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] -- in UExecCalc_Damage"), *ResistanceType.ToString());
		const FGameplayEffectAttributeCaptureDefinition ResistanceCaptureDef = GetAuraDamageStatics().TagsToCaptureDefs[ResistanceType];

		float DamageTypeValue = GetSafeSetByCaller(Spec, Pair.Key, 0.f, false);
		
		float ResistanceValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceCaptureDef, EvaluationParameters, ResistanceValue);
		ResistanceValue = FMath::Clamp(ResistanceValue, 0.f, 100.f);

		DamageTypeValue *= (100.f - ResistanceValue) / 100.f;

		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			if (TScriptInterface<ICombatInterface> CombatInterface = TargetAvatarActor)
			{
				CombatInterface->GetOnTakeDamageSignature().AddLambda([&DamageTypeValue](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatarActor,
				DamageTypeValue,
				0.f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatarActor,
				nullptr);
		}
		
		Damage += DamageTypeValue;
	}

	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().BlockChanceDef,
	                                                           EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	// If block, halve the damage
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	/*
	 * Armor Data
	 */

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().ArmorDef, EvaluationParameters,
	                                                           TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().ArmorPenetrationDef,
	                                                           EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	// 根据UCharacterClassInfo中存储的CurveTable获取Armor与ArmorPenetration的系数
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatarActor);

	FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficients = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	FRealCurve* ArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("Armor"), FString());
	const float ArmorCoefficients = ArmorCurve->Eval(TargetPlayerLevel);

	const float EffectiveArmor = TargetArmor *= FMath::Max<float>(
		0.f, (100 - SourceArmorPenetration * ArmorPenetrationCoefficients) / 100.f);
	Damage *= (100 - EffectiveArmor * ArmorCoefficients) / 100.f;

	/*
	 * Critical Hit Data
	 */

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().CriticalHitChanceDef,
	                                                           EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().CriticalHitDamageDef,
	                                                           EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().CriticalHitResistanceDef,
	                                                           EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficients = CriticalHitResistanceCurve->Eval(
		TargetPlayerLevel);

	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance *
		CriticalHitResistanceCoefficients;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	Damage = bCriticalHit ? Damage * 2.f + SourceCriticalHitDamage : Damage;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	/*
	 * Apply a final calculation result
	 */

	// 可以对多个Attribute进行设置
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
