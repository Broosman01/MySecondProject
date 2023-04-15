// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/BarricadeUnit.h"
#include "Units/HeroUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MyHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BarricadeLog, All, All);

void ABarricadeUnit::UnitIsDead() 
{
	UE_LOG(BarricadeLog, Error, TEXT("Barricade is broken!"));
	AHeroUnit* Player = Cast<AHeroUnit>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		float ADamage = Player->HealthComponent->GetMaxHealth();
		Player->TakeDamage(ADamage, FDamageEvent(), Controller, this);
	}
	SetLifeSpan(0.25f);
}

