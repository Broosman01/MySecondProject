// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Units/BaseUnit.h"
#include "UnitsLvlSaveGame.generated.h"

UCLASS()
class AODD_API UUnitsLvlSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EUnitType, int32> UnitsLvl;
};
