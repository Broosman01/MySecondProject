// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/BaseUnit.h"
#include "MeleeUnit.generated.h"

class AShockwave;

UCLASS()
class AODD_API AMeleeUnit : public ABaseUnit
{
	GENERATED_BODY()

public:
	virtual void AttackTarget(USkeletalMeshComponent* MeshComponent) override;
};
