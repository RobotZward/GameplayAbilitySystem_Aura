// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	// 这里采用UObject而不是UAuraWidgetController，其主要原因是我们可以传入继承自UObject的任何对象
	// 只需在该对象中创建动态多播代理，就能够在UserWidget中进行绑定，不需要受限于UAuraWidgetController类
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
