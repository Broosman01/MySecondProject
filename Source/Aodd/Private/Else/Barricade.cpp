// Fill out your copyright notice in the Description page of Project Settings.


#include "Else/Barricade.h"
#include "Components/MyHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/UnitHealthWidget.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"

ABarricade::ABarricade()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);

	HealthComponent = CreateDefaultSubobject<UMyHealthComponent>("HealthComponent");

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(GetRootComponent());

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
}

void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnHealthChanged.AddUObject(this, &ABarricade::ChangeHealth);
	HealthWidgetComponent->SetVisibility(false);
}

void ABarricade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABarricade::ChangeHealth(float Health)
{
	HealthWidgetComponent->SetVisibility(true);

	if (HealthChangeTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthChangeTimer);
	}

	GetWorld()->GetTimerManager().SetTimer(HealthChangeTimer, [&]()
		{
			GetWorld()->GetTimerManager().ClearTimer(HealthChangeTimer);
			HealthWidgetComponent->SetVisibility(false);
		}, 2.0f, false);
}

