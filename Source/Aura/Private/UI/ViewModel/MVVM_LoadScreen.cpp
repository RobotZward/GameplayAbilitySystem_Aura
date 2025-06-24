// Copyright HigeNeko


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index)
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 SlotIndex)
{
	LoadSlots[SlotIndex]->OnSetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 SlotIndex, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[SlotIndex]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[SlotIndex]->SetPlayerName(EnteredName);
	LoadSlots[SlotIndex]->SlotStatus = Taken;
	LoadSlots[SlotIndex]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	
	AuraGameMode->SaveSlotData(LoadSlots[SlotIndex], SlotIndex);
	LoadSlots[SlotIndex]->InitializeSlot();

	AuraGameMode->SaveDataToGameInstance(LoadSlots[SlotIndex]);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 SlotIndex)
{
	OnSlotSelectedDelegate.Broadcast();
	for (const TPair<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == SlotIndex)
		{
			LoadSlot.Value->OnEnableSelectSlotButtonDelegate.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->OnEnableSelectSlotButtonDelegate.Broadcast(true);
		}
	}
	SelectedLoadSlot = LoadSlots[SlotIndex];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedLoadSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedLoadSlot, SelectedLoadSlot->SlotIndex);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	if (IsValid(SelectedLoadSlot))
	{
		AuraGameMode->TravelToMap(SelectedLoadSlot);
		AuraGameMode->SaveDataToGameInstance(SelectedLoadSlot);
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TPair<int32, UMVVM_LoadSlot*>& Pair : LoadSlots)
	{
		ULoadScreenSaveGame* SaveGameObject = AuraGameMode->LoadSlotData(Pair.Value->GetLoadSlotName(), Pair.Key);

		const FString MapName = SaveGameObject->MapName;
		const FString PlayerName = SaveGameObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveGameObject->SaveSlotStatus;
		const FName PlayerStartTag = SaveGameObject->PlayerStartTag;

		Pair.Value->SetMapName(MapName);
		Pair.Value->SetPlayerName(PlayerName);
		Pair.Value->SlotStatus = SaveSlotStatus;
		Pair.Value->PlayerStartTag = PlayerStartTag;
		
		Pair.Value->InitializeSlot();
	}
}
