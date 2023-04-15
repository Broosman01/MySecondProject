// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LvlUpComponent.h"

ULvlUpComponent::ULvlUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULvlUpComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupStats();
}

void ULvlUpComponent::SetupStats()
{
	HealthByLvl(CurrentLvl);
	DamageByLvl(CurrentLvl);
	OnChangeLvl.Broadcast();
}

float ULvlUpComponent::HealthByLvl(int32 Lvl)
{
	CurrentHealth = float(FMath::Max(1, Lvl)) / float(FMath::Max(1, MaxLvl)) * float(MaxHealth);
	return CurrentHealth;
}

float ULvlUpComponent::DamageByLvl(int32 Lvl)
{
	CurrentDamage = float(FMath::Max(1, Lvl)) / float(FMath::Max(1, MaxLvl)) * float(MaxDamage);
	return CurrentDamage;
}

float ULvlUpComponent::GoldForUpByLvl(int32 Lvl)
{
	int32 NeedGold = 0;
	for (int32 i = 1; i <= Lvl; i++)
	{
		NeedGold = (float(NeedGold) + 100.0f * i * i) * float(FMath::Max(1, Lvl)) / float(FMath::Max(1, MaxLvl));
	}
	return NeedGold;
}

void ULvlUpComponent::LvlUp()
{
	if (CurrentLvl >= MaxLvl || CurrentGold < GoldForUpByLvl(CurrentLvl)) return;
	CurrentGold -= GoldForUpByLvl(CurrentLvl);
	CurrentLvl++;
	SetupStats();
}