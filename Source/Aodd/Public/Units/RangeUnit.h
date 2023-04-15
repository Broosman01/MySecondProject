// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/BaseUnit.h"
#include "RangeUnit.generated.h"

class AUnitProjectile;

UCLASS()
class AODD_API ARangeUnit : public ABaseUnit
{
	GENERATED_BODY()
	
public:
	virtual void AttackTarget(USkeletalMeshComponent* MeshComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AUnitProjectile> UnitProjectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName FireSocketName = "FireSocket";

	void SpawnProjectile();
};
