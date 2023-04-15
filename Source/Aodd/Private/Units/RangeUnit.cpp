// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/RangeUnit.h"
#include "Components/MyHealthComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Units/BaseUnitAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Units/Projectiles/UnitProjectile.h"

void ARangeUnit::AttackTarget(USkeletalMeshComponent* MeshComponent)
{
	if (GetMesh() != MeshComponent || !IsSomeEnemyInRange())
	{
		if (UnitController)
		{
			UnitController->GetBlackboardComponent()->SetValueAsBool(TEXT("HasEnemy"), IsSomeEnemyInRange());
		}
		ClearTimerManager();
		return;
	}

	FindTarget();
	SpawnProjectile();

	if (EnemyList[EnemyIndex]->HealthComponent->IsDead())
	{
		EnemyList.Remove(EnemyTarget);
		UnitController->GetBlackboardComponent()->SetValueAsBool(TEXT("HasEnemy"), IsSomeEnemyInRange());
		ClearTimerManager();
		return;
	}
}

void ARangeUnit::SpawnProjectile()
{
	FVector ProjectileLocation = GetActorLocation();
	FRotator ProjectileRotation = GetActorRotation();
	FVector ProjectileScale = FVector(1.0f, 1.0f, 1.0f);
	const FTransform ProjectileTransform(ProjectileRotation, ProjectileLocation, ProjectileScale);
	FActorSpawnParameters SpawnInfo;

	auto SpawnedProjectile = GetWorld()->SpawnActor<AUnitProjectile>(UnitProjectile, ProjectileLocation, ProjectileRotation, SpawnInfo);

	SpawnedProjectile->EnemyTarget = EnemyList[EnemyIndex];
	SpawnedProjectile->ProjectileDamage = Damage;
	SpawnedProjectile->StartLoc = GetMesh()->GetSocketLocation(FireSocketName);
	SpawnedProjectile->SetupProjectileParams();
}