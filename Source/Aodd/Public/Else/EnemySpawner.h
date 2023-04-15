// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class ABaseUnit;

UCLASS()
class AODD_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemyList")
	TArray<TSubclassOf<ABaseUnit>> EnemyClassList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemyList")
	TArray<TSubclassOf<ABaseUnit>> CurrentEnemyClassList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemyList", meta = (ClampMin = "1", ClampMax = "10"))
	int32 CurrentLvl = 1;

	int32 MaxLvl = 50;

	int32 MaxEnemyCount = 50;
	int32 CurrentEnemyCount = 0;
	int32 EnemyNumber = 1;
	int32 Seed = 0;
private:
	void StartSpawnEnemy();
	void StopSpawnEnemy();

	void SetupEnemyList();
	void SpawnEnemy();

	int32 GetEnemyNumber(int32 Lvl);

};
