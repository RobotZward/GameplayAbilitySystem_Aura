// Copyright HigeNeko


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound)
{
	// ReSharper disable once CppUseStructuredBinding
	// 在这里使用结构化绑定不是标准的C++写法，存在很大风险
	for (const auto& InputAction : AbilityInputActions)
	{
		if(InputAction.InputAction && InputAction.InputTag.MatchesTagExact(InputTag))
		{
			return InputAction.InputAction;
		}
	}
	
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find GameplayTag [%s] in UAuraInputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
