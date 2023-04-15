// Copyright Epic Games, Inc. All Rights Reserved.


#include "AoddGameModeBase.h"
#include "Units/HeroUnit.h"
#include "MainHero/MyMainHeroPlayerController.h"
#include "SaveGame/SelectedLvlSaveGame.h"
#include "SaveGame/CompleteLvlsSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/AoddHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogAoddGM, All, All);

AAoddGameModeBase::AAoddGameModeBase()
{
	DefaultPawnClass = AHeroUnit::StaticClass();
	PlayerControllerClass = AMyMainHeroPlayerController::StaticClass();
	HUDClass = AAoddHUD::StaticClass();
}

void AAoddGameModeBase::StartPlay()
{
	Super::StartPlay();
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("SelectedLvl"), 0))
	{
		return;
	}
	USelectedLvlSaveGame* SaveGameRef = Cast<USelectedLvlSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SelectedLvl"), 0));
	if (!SaveGameRef) return;
	CurrentLvl = SaveGameRef->CurrentLvl;
}

bool AAoddGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		OnPauseSet.Broadcast();
	}
	return PauseSet;
}

bool AAoddGameModeBase::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		OnPauseReset.Broadcast();
	}
	return PauseCleared;
}

void AAoddGameModeBase::SetWinMenu()
{
	if (Defeat) return;

	if (!UGameplayStatics::DoesSaveGameExist(TEXT("CompletedLvls"), 0))
	{
		UCompleteLvlsSaveGame* CompletedLvlsSG = Cast<UCompleteLvlsSaveGame>(UGameplayStatics::CreateSaveGameObject(UCompleteLvlsSaveGame::StaticClass()));
		CompletedLvlsSG->CompletedLvls = 1;
		UGameplayStatics::SaveGameToSlot(CompletedLvlsSG, TEXT("CompletedLvls"), 0);
	}

	if (UGameplayStatics::DoesSaveGameExist(TEXT("CompletedLvls"), 0))
	{
		UCompleteLvlsSaveGame* CompletedLvlsSG = Cast<UCompleteLvlsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("CompletedLvls"), 0));
		if (CompletedLvlsSG->CompletedLvls < CurrentLvl)
		{
			CompletedLvlsSG = Cast<UCompleteLvlsSaveGame>(UGameplayStatics::CreateSaveGameObject(UCompleteLvlsSaveGame::StaticClass()));
			CompletedLvlsSG->CompletedLvls = CurrentLvl;
			UGameplayStatics::SaveGameToSlot(CompletedLvlsSG, TEXT("CompletedLvls"), 0);
		}
	}
	OnWinMenuSet.Broadcast();
}

void AAoddGameModeBase::SetLoseMenu()
{
	LvlComplete = false;
	Defeat = true;
	OnLoseMenuSet.Broadcast();
}

int32 AAoddGameModeBase::EnemyCountByLvl(int32 Lvl)
{
	MaxEnemyOnLvl = 0;
	if(Lvl == 0) return MaxEnemyOnLvl;
	
	MaxEnemyOnLvl = float(MaxEnemyCountOnLastLvl) * powf((float(Lvl) / float(MaxLvl)), 0.7f);
	MaxEnemyOnLvl = FMath::Max(Lvl, MaxEnemyOnLvl);

	return MaxEnemyOnLvl;
}

void AAoddGameModeBase::KillEnemy()
{
	if (EnemyKilled < MaxEnemyOnLvl)
	{
		EnemyKilled++;

		UE_LOG(LogAoddGM, Error, TEXT("KilledEnemy:%d"), EnemyKilled);
		if (EnemyKilled >= MaxEnemyOnLvl)
		{
			SetWinMenu();
		}
	}
}
