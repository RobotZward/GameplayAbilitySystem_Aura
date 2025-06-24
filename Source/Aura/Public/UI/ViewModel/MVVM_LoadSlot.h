// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndexSignature, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButtonSignature, bool, bEnable);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndexSignature OnSetWidgetSwitcherIndexDelegate;
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButtonSignature OnEnableSelectSlotButtonDelegate;

	// 在生成WBP_LoadMenu时根据硬盘信息初始化每个存档槽中的数据
	void InitializeSlot();

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	void SetLoadSlotName(FString InSlotName);
	FString GetLoadSlotName() const { return LoadSlotName; }

	void SetPlayerName(FString InPlayerName);
	FString GetPlayerName() const { return PlayerName; }

	void SetMapName(FString InMapName);
	FString GetMapName() const { return MapName; }

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString LoadSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString MapName;
};
