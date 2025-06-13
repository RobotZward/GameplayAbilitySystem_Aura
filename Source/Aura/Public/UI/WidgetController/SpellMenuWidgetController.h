// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

struct FAuraGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool,
                                             bEquipbuttonEnabled);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

struct FGameplayTag;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|PlayerStat")
	FOnPlayerStatChangeSignature OnSpellPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellButton")
	FSpellGlobeSelectedSignature OnSpellGlobeSelectedDelegate;

	UFUNCTION(BlueprintCallable, Category = "GAS|SpellButton")
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

private:
	static void ShouldEnableButtons(
		const FGameplayTag& StatusTag,
		int32 SpellPoints,
		bool& bShouldEnableSpellPointsButton,
		bool& bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = {
		FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked
	};
	int32 CurrentSpellPoints = 0;
};
