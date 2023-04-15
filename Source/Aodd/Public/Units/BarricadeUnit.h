// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/BaseUnit.h"
#include "BarricadeUnit.generated.h"

UCLASS()
class AODD_API ABarricadeUnit : public ABaseUnit
{
	GENERATED_BODY()
private:
	virtual void UnitIsDead() override;
};
