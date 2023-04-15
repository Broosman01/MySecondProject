// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UMyHealthComponent::UMyHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);
	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UMyHealthComponent::OnTakeAnyDamage);
	}
}

void UMyHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead()) return;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

