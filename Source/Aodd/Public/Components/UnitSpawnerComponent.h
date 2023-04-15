// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitSpawnerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangeCurrentMetal)
DECLARE_MULTICAST_DELEGATE(FOnChangeAllStats)

class ABaseUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AODD_API UUnitSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitSpawnerComponent();

	void UpgradeMetalProducer();

	FOnChangeCurrentMetal OnMetalChanged;
	FOnChangeAllStats OnStatsChanged;

	int32 MaxMetal = 250;
	int32 CurrentMaxMetal = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CurrentMetal = 0;

	int32 MetalForUp = 0;

	int32 MaxProducerLvl = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	int32 CurrentProducerLvl = 1;

	int32 MaxMetalProdictionSpeed = 300;
	int32 CurrentMetalProdictionSpeed = 0;

	float GetMetalPercent() { return float(CurrentMetal) / float(CurrentMaxMetal); }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Units")
	TArray<TSubclassOf<ABaseUnit>> UnitList;
	ABaseUnit* MainHero;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnUnit();

private:
	void GetOwnerHero();

	FTimerHandle ProduceTimerHandle;

	float MetalProductionTime = 1.0f;

	void ProduceMetal();

	void SetupProduceTimer();
	void ClearProduceTimer();

	bool EnoughMetalForUpgrade();

	void SetupAllStats();

	void SetMetalProductionTime();
	void SetCurrentMaxMetal();
	void SetMetalForUp();

	UFUNCTION(BlueprintCallable)
	void ChangeCurrentMetal();
};
