// Copyright HigeNeko


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();

	AutoRun();
}

void AAuraPlayerController::ClientShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHt, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHt, bCriticalHit);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (APawn* ControlledPawn = GetPawn<APawn>(); ControlledPawn && bAutoRunning)
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToSplinePoint = FVector::Dist(LocationOnSpline, ControlledPawn->GetActorLocation());
		if (DistanceToSplinePoint <= AutoRunAcceptanceRadius)
		{
			Spline->RemoveSplinePoint(0);
		}
		const float DistanceToDestination = FVector::Dist(LocationOnSpline, CacheDestination);
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// check宏，用于当传入的变量非法时停止执行
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	// 鼠标配置
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(const FInputActionValue& InputActionValue,
                                                   const FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(const FInputActionValue& InputActionValue,
                                                    const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent()) GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		// 按下的是鼠标左键，但正瞄准敌人，释放技能
		if (GetAuraAbilitySystemComponent()) GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		const APawn* ControlledPawn = GetPawn<APawn>();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			// 使用FindPathToLocationSynchronously获取导航路径并将导航点添加至SplineLine
			// 要在Project Setting->Navigation System中勾选Allow Client Side Navigation
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this,
				ControlledPawn->GetActorLocation(),
				CacheDestination))
			{
				Spline->ClearSplinePoints();

				for (const FVector& PathPoint : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
				}
				CacheDestination = NavPath->PathPoints.Last();
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(const FInputActionValue& InputActionValue, const FGameplayTag InputTag)
{
	// 按下的不是鼠标左键，释放技能
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent()) GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		// 按下的是鼠标左键，但正瞄准敌人，释放技能
		if (GetAuraAbilitySystemComponent()) GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		// 按下的是鼠标左键，但没瞄准敌人，增加左键按下的持续时间，获取鼠标检测的位置
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit) CacheDestination = CursorHit.Location;

		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			const FVector WorldDirection = (CacheDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}
