// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AoddGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPauseSet)
DECLARE_MULTICAST_DELEGATE(FOnPauseReset)
DECLARE_MULTICAST_DELEGATE(FOnWinMenuSet)
DECLARE_MULTICAST_DELEGATE(FOnLoseMenuSet)

UCLASS()
class AODD_API AAoddGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAoddGameModeBase();

	FOnPauseSet OnPauseSet;
	FOnPauseReset OnPauseReset;
	FOnWinMenuSet OnWinMenuSet;
	FOnLoseMenuSet OnLoseMenuSet;

	bool LvlComplete = false;
	bool Defeat = false;
	void KillEnemy();

	void SetLoseMenu();
	
	int32 EnemyCountByLvl(int32 Lvl);

	int32 MinLvl = 1;
	int32 MaxLvl = 25;
protected:
	virtual void StartPlay() override;

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	void SetWinMenu(); 

	UPROPERTY(EditDefaultsOnly)
	int32 MaxEnemyCountOnLastLvl = 250;


	int32 MaxEnemyOnLvl = 0;
	int32 EnemyKilled = 0;

	int32 CurrentLvl = 0;
};
