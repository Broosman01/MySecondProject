// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UnitSpawnerComponent.h"
#include "Units/BaseUnit.h"
#include "Units/HeroUnit.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpawner, All, All);

UUnitSpawnerComponent::UUnitSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUnitSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwnerHero();
	SetupAllStats();

	SetupProduceTimer();
}

void UUnitSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUnitSpawnerComponent::SpawnUnit()
{
	FActorSpawnParameters SpawnInfo;

	FVector UnitLocation = GetOwner()->GetActorLocation();
	FRotator UnitRotation = GetOwner()->GetActorRotation();
	FVector UnitScale = FVector(1.0f, 1.0f, 1.0f);
	const FTransform ProjectileTransform(UnitRotation, UnitLocation, UnitScale);

	const auto ChoosenUnit = UnitList[0];
	auto SpawnedUnit = GetWorld()->SpawnActor<ABaseUnit>(ChoosenUnit, UnitLocation, UnitRotation, SpawnInfo);
}

void UUnitSpawnerComponent::SetupProduceTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ProduceTimerHandle ,this,&UUnitSpawnerComponent::ProduceMetal, MetalProductionTime, true);
}

void UUnitSpawnerComponent::ClearProduceTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ProduceTimerHandle);
}

void UUnitSpawnerComponent::ProduceMetal()
{
	if (CurrentMetal < CurrentMaxMetal)
	{
		CurrentMetal ++;
		OnMetalChanged.Broadcast();
	}
}

void UUnitSpawnerComponent::UpgradeMetalProducer()
{
	if (CurrentProducerLvl >= MaxProducerLvl) return;
	if (!EnoughMetalForUpgrade()) return;
	CurrentMetal -= MetalForUp;
	CurrentProducerLvl ++;

	ClearProduceTimer();

	SetupAllStats();

	SetupProduceTimer();
}

void UUnitSpawnerComponent::SetupAllStats()
{
	SetCurrentMaxMetal();
	SetMetalProductionTime();
	SetMetalForUp();

	OnStatsChanged.Broadcast();
}

void UUnitSpawnerComponent::SetMetalProductionTime()
{
	CurrentMetalProdictionSpeed = float(CurrentProducerLvl) / float(MaxProducerLvl) * MaxMetalProdictionSpeed;
	MetalProductionTime = 60.0f / CurrentMetalProdictionSpeed;
}

bool UUnitSpawnerComponent::EnoughMetalForUpgrade()
{
	if(CurrentMetal >= MetalForUp) return true;
	else return false;
}

void UUnitSpawnerComponent::SetCurrentMaxMetal()
{
	CurrentMaxMetal = float(MaxMetal) * float(CurrentProducerLvl) / float(MaxProducerLvl);
}

void UUnitSpawnerComponent::SetMetalForUp()
{
	MetalForUp = float(CurrentMaxMetal) * 0.75;
}

void UUnitSpawnerComponent::GetOwnerHero()
{
	const auto Player = GetOwner();
	if (!Player) return;
	MainHero = Cast<AHeroUnit>(Player);
}

void UUnitSpawnerComponent::ChangeCurrentMetal()
{
	OnMetalChanged.Broadcast();
}