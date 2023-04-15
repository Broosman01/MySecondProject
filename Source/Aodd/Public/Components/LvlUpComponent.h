// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LvlUpComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangeLvl)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AODD_API ULvlUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULvlUpComponent();

	FOnChangeLvl OnChangeLvl;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "10"))
	int32 CurrentLvl = 1;

	int32 CurrentHealth = 20;

	int32 CurrentDamage = 10;

	int32 MaxLvl = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxHealth = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxDamage = 50;

	void LvlUp();

	float DamageByLvl(int32 Lvl);
	float HealthByLvl(int32 Lvl);
	float GoldForUpByLvl(int32 Lvl);

protected:
	virtual void BeginPlay() override;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentGold = 10;

private:
	void SetupStats();
};
