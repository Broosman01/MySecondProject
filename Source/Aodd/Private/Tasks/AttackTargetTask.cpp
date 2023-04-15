// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/AttackTargetTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/MyHealthComponent.h"
#include "AIController.h"
#include "Units/BaseUnit.h"

UAttackTargetTask::UAttackTargetTask()
{
     NodeName = "Attack Target";
}

EBTNodeResult::Type UAttackTargetTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto BlackBoard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !BlackBoard) return EBTNodeResult::Failed;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	auto AIUnit = Cast<ABaseUnit>(Pawn);
	if (!AIUnit) return EBTNodeResult::Failed;

	if (!AIUnit->EnemyListValid())
	{
		BlackBoard->SetValueAsBool(TEXT("HasEnemy"), false);
		BlackBoard->SetValueAsBool(TEXT("IsAttacking"), false);
		return EBTNodeResult::Succeeded;
	}
	if (AIUnit->AttackAnimMontages.Num() <= 0)return EBTNodeResult::Failed;

	if (AIUnit->SplashAttack)
	{
		float NewRange = AIUnit->AttackRange * 1.6f;
		AIUnit->SetupAttackRange(NewRange);
	}
	int32 index = rand() % AIUnit->AttackAnimMontages.Num();
	AIUnit->PlayAnimMontage(AIUnit->AttackAnimMontages[index]);
	BlackBoard->SetValueAsBool(TEXT("IsAttacking"), true);
	return EBTNodeResult::Succeeded;
}