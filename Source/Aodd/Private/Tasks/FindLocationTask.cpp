// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/FindLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Units/BaseUnit.h"
#include "NavigationSystem.h"
#include "Else/MyEndPoint.h"
#include "Kismet/GameplayStatics.h"

UFindLocationTask::UFindLocationTask() 
{
	NodeName = "Next Location";
}

EBTNodeResult::Type UFindLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto BlackBoard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !BlackBoard) return EBTNodeResult::Failed;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSys) return EBTNodeResult::Failed;

	auto AIUnit = Cast<ABaseUnit>(Pawn);
	if (!AIUnit) return EBTNodeResult::Failed;
	if (AIUnit->SplashAttack)
	{
		float NewRange = AIUnit->AttackRange;
		AIUnit->SetupAttackRange(NewRange);
	}
	FNavLocation NavLocation;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EndPointClass, EndPoints);
	FVector AimLocation = EndPoints[0]->GetActorLocation();
	BlackBoard->SetValueAsVector(AimLocationKey.SelectedKeyName, AimLocation);
	return EBTNodeResult::Succeeded;
}

