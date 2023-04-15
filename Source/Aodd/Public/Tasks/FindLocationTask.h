// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindLocationTask.generated.h"

class AMyEndPoint;

UCLASS()
class AODD_API UFindLocationTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UFindLocationTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
 	//float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AMyEndPoint> EndPointClass;

	TArray<AActor*> EndPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector AimLocationKey;

};
