// Copyright HigeNeko


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoFromTag(const FGameplayTag& AttributeTag, bool bLogNitFound)
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNitFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find info for AttributeTag [%s] on AttributeInfo [%s]."),
		       *AttributeTag.ToString(),
		       *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}
