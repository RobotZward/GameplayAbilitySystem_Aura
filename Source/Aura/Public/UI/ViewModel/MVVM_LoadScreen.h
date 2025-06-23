// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelectedSignature);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	
	void InitializeLoadSlots();

	UPROPERTY(BlueprintAssignable)
	FSlotSelectedSignature OnSlotSelectedDelegate;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 SlotIndex, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

	void SetNumLoadSlots(int32 InNumLoadSlots);
	int32 GetNumLoadSlots() const { return NumLoadSlots; }

	// 遍历LoadSlots，从磁盘中读取存档并对LoadSlotViewModel赋值
	void LoadData();
	
private:
	
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedLoadSlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	int32 NumLoadSlots;
};
