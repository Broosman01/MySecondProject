// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/Projectiles/UnitProjectile.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

AUnitProjectile::AUnitProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileTimeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
}

void AUnitProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AUnitProjectile::SetupProjectileParams()
{
	UKismetMathLibrary::FindLookAtRotation(StartLoc, EnemyTarget->GetActorLocation());
	Distance = FVector::Dist(StartLoc, EnemyTarget->GetActorLocation());
	MaxDistance = FMath::Max(MaxDistance, Distance);
	MaxAmplitude = MaxDistance / 6.0f;
	Amplitude = FMath::Lerp(0.0f, MaxAmplitude, Distance / MaxDistance);

	TimelineTickEvent.BindUFunction(this, FName("UpdateTimeline"));
	TimelineFinishedEvent.BindUFunction(this, FName("FinishTimeline"));

	if (ProjectileCurveLocX && ProjectileCurveLocZ && ProjectileCurveRotX)
	{
		ProjectileTimeline->AddInterpFloat(ProjectileCurveLocX, ForwardLoc);
		ProjectileTimeline->AddInterpFloat(ProjectileCurveLocZ, UpLoc);
		ProjectileTimeline->AddInterpFloat(ProjectileCurveRotX, RotX);
	}

	ProjectileTimeline->SetPlayRate(MaxDistance / Distance);
	ProjectileTimeline->SetTimelinePostUpdateFunc(TimelineTickEvent);
	ProjectileTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
	ProjectileTimeline->PlayFromStart();
}

void AUnitProjectile::UpdateTimeline()
{
	SetProjectileTransform
	(
		ProjectileCurveLocX->GetFloatValue(ProjectileTimeline->GetPlaybackPosition()),
		ProjectileCurveLocZ->GetFloatValue(ProjectileTimeline->GetPlaybackPosition()),
		ProjectileCurveRotX->GetFloatValue(ProjectileTimeline->GetPlaybackPosition())
	);
}

void AUnitProjectile::SetProjectileTransform(float Forward, float Up, float Rot)
{
	FVector CurrentLocationForward = Forward * GetActorForwardVector() * Distance;
	FVector CurrentLocationUP = GetActorUpVector() * Amplitude * Up;

	FVector CurrentLocation = StartLoc + CurrentLocationForward + CurrentLocationUP;
	float CurrentAngel = FMath::Lerp(0.0f, 45.0f, Distance/MaxDistance) * (-1.0f) * Rot;
	FRotator CurrentRotation = FRotator(0.0f, -90.0f, CurrentAngel);

	SetActorLocation(CurrentLocation);
	ProjectileMesh->SetRelativeRotation(CurrentRotation);
}

void AUnitProjectile::FinishTimeline()
{
	EnemyTarget->TakeDamage(ProjectileDamage, FDamageEvent(), nullptr, this);
	Destroy();
}