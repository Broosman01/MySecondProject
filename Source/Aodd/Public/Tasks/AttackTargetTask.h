// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackTargetTask.generated.h"


UCLASS()
class AODD_API UAttackTargetTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UAttackTargetTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
