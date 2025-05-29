// Copyright HigeNeko


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair:AuraAttributeSet->TagsToAttributesFuncPtrTemplate)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoFromTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AuraAttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetStrengthAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetIntelligenceAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetResilienceAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetVigorAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetArmorAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetArmorPenetrationAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetBlockChanceAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetCriticalHitChanceAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetCriticalHitDamageAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetCriticalHitResistanceAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthRegenerationAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaRegenerationAttribute()).
	AddLambda(
		[this](const FOnAttributeChangeData& data)
		{
			
		}	
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
	                        AddLambda(
		                        [this](const FOnAttributeChangeData& Data)
		                        {
			                        // OnMaxHealthChanged.Broadcast(Data.NewValue);
		                        }
	                        );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).
	                        AddLambda(
		                        [this](const FOnAttributeChangeData& Data)
		                        {
			                        // OnMaxManaChanged.Broadcast(Data.NewValue);
		                        }
	                        );
}
