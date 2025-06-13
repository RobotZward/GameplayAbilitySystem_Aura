// Copyright HigeNeko


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	for (auto& Pair : GetAuraAttributeSet()->TagsToAttributesFuncPtrTemplate)
	{
		const FGameplayTag Tag = Pair.Key;
		const FGameplayAttribute Attribute = Pair.Value();
		
		BroadcastAttributeInfo(Tag, Attribute);
	}
	
	OnAttributePointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	for (auto& Pair : GetAuraAttributeSet()->TagsToAttributesFuncPtrTemplate)
	{
		const FGameplayTag Tag = Pair.Key;
		const FGameplayAttribute Attribute = Pair.Value();

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Tag, Attribute](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag, Attribute);
			});
	}
	
	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			OnAttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag,
                                                            const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoFromTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
