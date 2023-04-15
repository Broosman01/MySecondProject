// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/DoDamageNotify.h"

void UDoDamageNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}