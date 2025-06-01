// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	// 每帧调用来检测鼠标命中的Actor并高亮
	void CursorTrace();
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	FHitResult CursorHit;

	// 用于在AuraInputAction->SetupInputComponent中与InputConfig中的每一个InputAction进行绑定
	void AbilityInputTagPressed(const FInputActionValue& InputActionValue, const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FInputActionValue& InputActionValue, const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FInputActionValue& InputActionValue, const FGameplayTag InputTag);

	// 记录了InputAction及其对应的GameplayTag
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();

	/*
	 * 点击移动的配置参数
	 */
	FVector CacheDestination = FVector::ZeroVector;	// 点击时缓存鼠标命中的目标
	float FollowTime = 0.f;	// 记录鼠标按下后保持的时间
	float ShortPressThreshold = 0.5f;	// 持续时间的阈值，超出之后就不认为进行点击移动
	bool bAutoRunning = false;	// 每帧判断是否移动
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;	// 点击移动的到点范围

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
};
