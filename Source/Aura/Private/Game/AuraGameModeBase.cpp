// Copyright HigeNeko


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	DeleteSlot(LoadSlot, SlotIndex);
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->SaveSlotStatus = LoadSlot->SlotStatus;
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

ULoadScreenSaveGame* AAuraGameModeBase::LoadSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlot(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
		LoadSlot->SlotStatus = Vacant;
		const int32 WidgetSwitcherIndex = LoadSlot->SlotStatus.GetIntValue();
		LoadSlot->OnSetWidgetSwitcherIndexDelegate.Broadcast(WidgetSwitcherIndex);
		LoadSlot->OnEnableSelectSlotButtonDelegate.Broadcast(true);
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* LoadSlot)
{
	const FString MapName = LoadSlot->GetMapName();
	const int32 SlotIndex = LoadSlot->SlotIndex;
	const TSoftObjectPtr<UWorld> SlotMap = Maps.FindChecked(MapName);

	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, SlotMap);
}

void AAuraGameModeBase::SaveDataToGameInstance(UMVVM_LoadSlot* LoadSlot)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	AuraGameInstance->LoadSlotName = LoadSlot->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = LoadSlot->SlotIndex;
	AuraGameInstance->PlayerStartTag = LoadSlot->PlayerStartTag;
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	const FName& PlayerStartTag = AuraGameInstance->PlayerStartTag;
	
	// 使用GetAllActorsOfClass获取APlayerStart
	{
	// 	TArray<AActor*> Actors;
	// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	// 	if (Actors.Num() > 0)
	// 	{
	// 		AActor* SelectedActor = Actors[0];
	// 		for (AActor* Actor : Actors)
	// 		{
	// 			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
	// 			{
	// 				if (PlayerStart->PlayerStartTag == FName("TheTag"))
	// 				{
	// 					SelectedActor = Actor;
	// 					break;
	// 				}
	// 			}
	// 		}
	// 		return SelectedActor;
	// 	}	
	// 	return nullptr;
	}

	// 使用TActorIterator获取APlayerStart
	{
		APlayerStart* FoundPlayerStart = nullptr;
		
		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			APlayerStart* PlayerStart = *It;
			FoundPlayerStart = PlayerStart;
			
			if (PlayerStart->PlayerStartTag == PlayerStartTag)
			{
				return PlayerStart;
			}
		}
		return FoundPlayerStart;
	}
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
