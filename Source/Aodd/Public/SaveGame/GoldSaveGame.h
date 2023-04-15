// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GoldSaveGame.generated.h"


UCLASS()
class AODD_API UGoldSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 HeroGold = 0;
};
