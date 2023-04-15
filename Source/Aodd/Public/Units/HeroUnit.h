// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/BaseUnit.h"
#include "HeroUnit.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangeIcons)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeGold, int32)

class UCameraComponent;
class USpringArmComponent;
class UUnitSpawnerComponent;

UCLASS()
class AODD_API AHeroUnit : public ABaseUnit
{
	GENERATED_BODY()

public:
	AHeroUnit();

	void AddGold(int32 Money);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Money")
	int32 Gold = 0;

	FOnChangeIcons OnChangeIcons;
	FOnChangeGold OnChangeGold;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UUnitSpawnerComponent* UnitSpawnerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Units")
	TArray<EUnitType> ChoosenUnits;

	virtual void UnitIsDead() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

private:
	void MoveForward(float Amount);

	void SetUpTimer();

	virtual void EnemyInAttackRange(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void EnemyOutAttackRange(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex);

	void AttackEnemy();
	void ClearTimerManager();
	void LoadGold();

	bool CanFire = false;
};
