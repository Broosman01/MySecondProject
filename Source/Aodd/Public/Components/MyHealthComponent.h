// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AODD_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMyHealthComponent();

	float GetHealth() const {return Health;}
	float GetMaxHealth() const {return MaxHealth;}

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return Health <= 0.0f; }

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	float Health = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealthPercent() const { return Health / MaxHealth; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};