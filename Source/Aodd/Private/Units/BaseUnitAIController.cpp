// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/BaseUnitAIController.h"
#include "Units/BaseUnit.h"

void ABaseUnitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); 

	const auto AICharacter = Cast<ABaseUnit>(InPawn);
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}