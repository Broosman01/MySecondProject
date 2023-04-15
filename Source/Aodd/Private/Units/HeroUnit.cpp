// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/HeroUnit.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/MyHealthComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/UnitSpawnerComponent.h"
#include "Aodd/AoddGameModeBase.h"
#include "SaveGame/SelectedUnitsSaveGame.h"
#include "SaveGame/UnitsLvlSaveGame.h"
#include "SaveGame/GoldSaveGame.h"

DEFINE_LOG_CATEGORY_STATIC(BaseHeroLog, All, All);

AHeroUnit::AHeroUnit()
{
	UnitSpawnerComponent = CreateDefaultSubobject<UUnitSpawnerComponent>("UnitSpawnerComponent");

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AHeroUnit::BeginPlay()
{
	Super::BeginPlay();
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("SelectedUnits"), 0))
	{
		return;
	}
	USelectedUnitsSaveGame* SaveGameRef = Cast<USelectedUnitsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SelectedUnits"), 0));
	ChoosenUnits = SaveGameRef->SelectedUnits;

	OnChangeIcons.Broadcast();
	LoadGold();
	HealthComponent->OnHealthChanged.Broadcast(HealthComponent->Health);
}


void AHeroUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHeroUnit::MoveForward);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AHeroUnit::SetUpTimer);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHeroUnit::ClearTimerManager);
	PlayerInputComponent->BindAction("UpgradeMetalProducer", IE_Pressed, UnitSpawnerComponent, &UUnitSpawnerComponent::UpgradeMetalProducer);
}

void AHeroUnit::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector(), Amount);
	if (Amount < 0)
	{
		CanFire = false;
		GetMesh()->USkeletalMeshComponent::SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
	else
	{
		CanFire = true;
		GetMesh()->USkeletalMeshComponent::SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	}
}

void AHeroUnit::EnemyInAttackRange(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HealthComponent->IsDead()) return;

	auto Enemy = Cast<ABaseUnit>(OtherActor);
	if (Enemy->HealthComponent->IsDead()) return;
	
	EnemyList.Add(Enemy);
	if (!CanFire) return;
	if(EnemyList.Num() == 1)
	{ 
		SetUpTimer();
	}
}

void AHeroUnit::EnemyOutAttackRange(UPrimitiveComponent* Comp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HealthComponent->IsDead()) return;
	auto  Enemy = Cast<ABaseUnit>(OtherActor);
	EnemyList.Remove(Enemy);
	if (!IsSomeEnemyInRange())
	{
		ClearTimerManager();
	}
}

void AHeroUnit::SetUpTimer()
{
	if (!IsSomeEnemyInRange())
	{
		ClearTimerManager();
		return;
	}
	if (AttackTimerHandle.IsValid())
	{
		ClearTimerManager();
	}
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AHeroUnit::AttackEnemy, 0.6f, true);
}

void AHeroUnit::AttackEnemy()
{
	if (HealthComponent->IsDead() || !IsSomeEnemyInRange())
	{
		ClearTimerManager();
		return;
	}

	FindTarget();
	auto TargetEnemy = EnemyList[EnemyIndex];
	if (!TargetEnemy) return;
	
	TargetEnemy->TakeDamage(Damage, FDamageEvent(), Controller, this);

	int32 index = rand()% AttackAnimMontages.Num();
	PlayAnimMontage(AttackAnimMontages[index]);

	if (TargetEnemy->HealthComponent->GetHealth() <= 0)
	{
		EnemyList.Remove(TargetEnemy);
		if (!IsSomeEnemyInRange())
		{
			ClearTimerManager();
		}
	}
}

void AHeroUnit::ClearTimerManager()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void AHeroUnit::UnitIsDead()
{
	ClearTimerManager();
	if (DeadAnimMontages.Num() > 0)
	{
		int32 index = rand() % DeadAnimMontages.Num();
		PlayAnimMontage(DeadAnimMontages[index]);
	}
	GetCharacterMovement()->DisableMovement();
	DisableInput(nullptr);
	auto GameMode = Cast<AAoddGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->SetLoseMenu();
}

void AHeroUnit::LoadGold()
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("Gold"), 0)) return;
	
	auto GoldSGRef = Cast<UGoldSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Gold"), 0));
	if (!GoldSGRef) return;
	Gold = GoldSGRef->HeroGold;
	OnChangeGold.Broadcast(Gold);
}

void AHeroUnit::AddGold(int32 Money)
{
	if (Money <= 0)return;
	Gold += Money;
	OnChangeGold.Broadcast(Gold);
}
