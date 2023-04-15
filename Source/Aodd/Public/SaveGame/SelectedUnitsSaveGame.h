// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Units/BaseUnit.h"
#include "SelectedUnitsSaveGame.generated.h"

UCLASS()
class AODD_API USelectedUnitsSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<EUnitType> SelectedUnits;
};
