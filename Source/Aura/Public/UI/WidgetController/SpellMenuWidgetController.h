// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

struct FAuraGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature,
	bool, bSpendPointsButtonEnabled,
	bool, bEquipbuttonEnabled,
	FString, Description,
	FString, NextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityTag);

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

	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenuController")
	FSpellGlobeSelectedSignature OnSpellGlobeSelectedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenuController")
	FWaitForEquipSelectionSignature OnWaitForEquipSelectionDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenuController")
	FWaitForEquipSelectionSignature OnStopWaitForEquipSelectionDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenuController")
	FSpellGlobeReassignedSignature OnReassignedDelegate;

	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenuController")
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenuController")
	void SpellGlobeDeselect();

	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenuController")
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenuController")
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenuController")
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot);

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
	bool bWaitingForEquipSelection = false;
	FGameplayTag SelectedSlot;
};
