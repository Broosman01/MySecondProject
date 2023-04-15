// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Barricade.generated.h"

class UMyHealthComponent;
class UWidgetComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class AODD_API ABarricade : public AActor
{
	GENERATED_BODY()
	
public:	
	ABarricade();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UMyHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollision;

	FTimerHandle HealthChangeTimer;

	virtual void BeginPlay() override;

private:
	void ChangeHealth(float Health);
};
