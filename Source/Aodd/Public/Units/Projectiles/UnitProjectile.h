// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "UnitProjectile.generated.h"

class UTimelineComponent;

UCLASS()
class AODD_API AUnitProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnitProjectile();

	AActor* EnemyTarget;

	FVector StartLoc;
protected:
	virtual void BeginPlay() override;

	USceneComponent* RootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Distance = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxDistance = 1000;

	float Amplitude;
	float MaxAmplitude;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetupProjectileParams();
	float ProjectileDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTimelineComponent* ProjectileTimeline;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* ProjectileCurveLocX;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* ProjectileCurveLocZ;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* ProjectileCurveRotX;

	FVector ActorLocation;
	FVector TargetLocation;

private:
	FOnTimelineFloat ForwardLoc;
	FOnTimelineFloat UpLoc;
	FOnTimelineFloat RotX;

	FOnTimelineEvent TimelineTickEvent;
	FOnTimelineEvent TimelineFinishedEvent;

	FVector EndLoc;

	UFUNCTION()
	void UpdateTimeline();

	UFUNCTION()
	void FinishTimeline();

	void SetProjectileTransform(float Forward, float Up, float Rot);
};
