// Copyright HigeNeko


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : AuraAttributeSet->TagsToAttributesFuncPtrTemplate)
	{
		const FGameplayTag Tag = Pair.Key;
		const FGameplayAttribute Attribute = Pair.Value();
		
		BroadcastAttributeInfo(Tag, Attribute);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Pair : AuraAttributeSet->TagsToAttributesFuncPtrTemplate)
	{
		const FGameplayTag Tag = Pair.Key;
		const FGameplayAttribute Attribute = Pair.Value();

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Tag, Attribute](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag, Attribute);
			});
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag,
                                                            const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoFromTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
