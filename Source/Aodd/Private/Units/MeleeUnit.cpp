// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/MeleeUnit.h"
#include "Components/MyHealthComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Units/BaseUnitAIController.h"
#include "Components/BoxComponent.h"


void AMeleeUnit::AttackTarget(USkeletalMeshComponent* MeshComponent)
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
	if (SplashAttack)
	{
		auto CurrentEnemyList = EnemyList;
		for (auto Enemy : CurrentEnemyList)
		{
			Enemy->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
			if (Enemy->HealthComponent->IsDead())
			{
				EnemyList.Remove(Enemy);
			}
		}
		UnitController->GetBlackboardComponent()->SetValueAsBool(TEXT("HasEnemy"), IsSomeEnemyInRange());
		ClearTimerManager();
		return;
	}

	else
	{
		FindTarget();
		EnemyTarget = EnemyList[EnemyIndex];
		EnemyTarget->TakeDamage(Damage, FDamageEvent(), Controller, this);

		if (EnemyList[EnemyIndex]->HealthComponent->IsDead())
		{
			EnemyList.Remove(EnemyTarget);
			UnitController->GetBlackboardComponent()->SetValueAsBool(TEXT("HasEnemy"), IsSomeEnemyInRange());
			ClearTimerManager();
			return;
		}
	}
}