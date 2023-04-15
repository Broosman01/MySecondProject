// Fill out your copyright notice in the Description page of Project Settings.


#include "Else/EnemySpawner.h"
#include "Aodd/AoddGameModeBase.h"
#include "Units/BaseUnit.h"
#include "SaveGame/SelectedLvlSaveGame.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemySpawner, All, All);

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("SelectedLvl"), 0))
	{
		return;
	}
	USelectedLvlSaveGame* SaveGameRef = Cast<USelectedLvlSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SelectedLvl"), 0));
	if (!SaveGameRef) return;

	const auto GameMode = Cast<AAoddGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	CurrentEnemyCount = 0;
	CurrentLvl = SaveGameRef->CurrentLvl;
	MaxLvl = GameMode->MaxLvl;
	MaxEnemyCount = GameMode->EnemyCountByLvl(CurrentLvl);
	Seed = CurrentLvl;
	EnemyNumber = CurrentLvl;

	UE_LOG(LogEnemySpawner, Error, TEXT("CurrentLvl:%d"), CurrentLvl);
	StartSpawnEnemy();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SetupEnemyList()
{
	float b = float(CurrentLvl) / float(MaxLvl);

	int32 a = b * float(EnemyClassList.Num());
	a = FMath::Max(0, a - 1);
	if (!EnemyClassList.IsValidIndex(a)) return;

	auto MainEnemy = EnemyClassList[a];
	if (!MainEnemy) return;

	CurrentEnemyClassList.Add(MainEnemy);

	for (int32 i = a; i < a + 4; i++)
	{
		if (!EnemyClassList.IsValidIndex(i)) break;

		auto Enemy = EnemyClassList[i];
		CurrentEnemyClassList.Add(Enemy);
	}
	for (int32 i = a; i > a - 4; i--)
	{
		if (!EnemyClassList.IsValidIndex(i)) break;

		auto Enemy = EnemyClassList[i];
		CurrentEnemyClassList.Add(Enemy);
	}

	for (auto Enemyname : CurrentEnemyClassList)
	{
		Enemyname->GetName();
		UE_LOG(LogEnemySpawner, Error, TEXT(":%s"), *Enemyname->GetName());
	}
}

void AEnemySpawner::StartSpawnEnemy()
{
	SetupEnemyList();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, 1.5f, true);
	UE_LOG(LogEnemySpawner, Error, TEXT("MaxEnemyCount:%d"), MaxEnemyCount);
}

void AEnemySpawner::StopSpawnEnemy()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

void AEnemySpawner::SpawnEnemy()
{
	if (CurrentEnemyClassList.Num() <= 0) return;

	auto EnemyClass = CurrentEnemyClassList[GetEnemyNumber(CurrentLvl)];

	if (!EnemyClass) return;
	
	const FTransform SpawnTransform(GetActorForwardVector().Rotation(), GetActorLocation());
	auto Enemy = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), EnemyClass, SpawnTransform);
	
	UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);

	UE_LOG(LogEnemySpawner, Error, TEXT(":%s"), *Enemy->GetName());
	CurrentEnemyCount++;
	if (CurrentEnemyCount >= MaxEnemyCount)
	{
		StopSpawnEnemy();
	}
}

int32 AEnemySpawner::GetEnemyNumber(int32 Lvl)
{
	if (CurrentEnemyClassList.Num() <= 0) return EnemyNumber;
	
	EnemyNumber = (43 * EnemyNumber + (EnemyNumber % 67)) % 1000000;
	int32 a = EnemyNumber % CurrentEnemyClassList.Num();
	UE_LOG(LogEnemySpawner, Error, TEXT("EnemyNumber:%d"), EnemyNumber);
	return a;
}