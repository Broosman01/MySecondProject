// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/BaseUnit.h"
#include "Units/HeroUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Aodd/AoddGameModeBase.h"
#include "Components/MyHealthComponent.h"
#include "Components/LvlUpComponent.h"
#include "UI/UnitHealthWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Animation/AnimBlueprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Units/BaseUnitAIController.h"
#include "Animations/AttackFinishedNotify.h"
#include "Animations/DoDamageNotify.h"
#include "Components/WidgetComponent.h"
#include "SaveGame/UnitsLvlSaveGame.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseUnit, All, All);

ABaseUnit::ABaseUnit() : UnitType(EUnitType::EUT_Worker)
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UMyHealthComponent>("HealthComponent");

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseUnit::EnemyInAttackRange);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseUnit::EnemyOutAttackRange);

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());

	LvlUpComponent = CreateDefaultSubobject<ULvlUpComponent>("LvlUpComponent");
}

void ABaseUnit::OnConstruction(const FTransform& Transform)
{
	FString UnitTablePath(TEXT("DataTable'/Game/DataTables/Units_DT.Units_DT'"));
	UnitTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *UnitTablePath));
	if (UnitTableObject)
	{
		UnitRow = nullptr;
		FString EnumName = UEnum::GetValueAsString(UnitType);
		FString CurrentName = EnumName.Replace(TEXT("EUnitType::EUT_"), TEXT(""), ESearchCase::IgnoreCase);

		UnitRow = UnitTableObject->FindRow<FUnitTable>(FName(*CurrentName), TEXT(""));
		if (UnitRow)
		{
			Icon = UnitRow->UnitIcon;
			Name = UnitRow->UnitName;
			IsEnemy = UnitRow->IsEnemyUnit;
			Cost = UnitRow->UnitCost;

			LvlUpComponent->MaxLvl = UnitRow->UnitMaxLevel;
			LvlUpComponent->MaxHealth = UnitRow->UnitMaxHealth;
			LvlUpComponent->MaxDamage = UnitRow->UnitMaxDamage;

			GetMesh()->SetSkeletalMesh(UnitRow->UnitMesh);
			GetCharacterMovement()->MaxWalkSpeed = UnitRow->UnitMoveSpeed;
			AttackAnimMontages = UnitRow->AttackAnimMontages;
			DeadAnimMontages = UnitRow->DeadAnimMontages;
			if (UnitRow->UnitAnim_BP)
			{
				GetMesh()->SetAnimInstanceClass(UnitRow->UnitAnim_BP);
			}
			AttackRange = UnitRow->AttackRange;
		}
	}
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();

	LoadUnitLvls();
	check(HealthComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &ABaseUnit::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &ABaseUnit::UnitIsDead);

	UnitController = Cast<ABaseUnitAIController>(GetController());

	HealthWidgetComponent->SetVisibility(false);
	UUnitHealthWidget* PlayerHUD = Cast<UUnitHealthWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (PlayerHUD)
	{
		PlayerHUD->PlayerOwner = this;
	}

	SetupAttackRange(AttackRange);
	InitAnimations();
	SetupDamageAndHealth();
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseUnit::LoadUnitLvls()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("UnitLvls"), 0))
	{
		UpSaveGame = Cast<UUnitsLvlSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("UnitLvls"), 0));
	}
}

void ABaseUnit::SetupAttackRange(float Range)
{
	BoxCollision->SetRelativeLocation(FVector(Range, 0.0f, 0.0f));
	BoxCollision->SetBoxExtent(FVector(Range, 200.0f, 200.0f));
}

void ABaseUnit::SetupDamageAndHealth()
{
	if (!UnitRow) return;

	int32 UnitLvl = 1;
	if (UpSaveGame)
	{
		UnitLvl = UpSaveGame->UnitsLvl.FindRef(UnitType);
	}
	LvlUpComponent->MaxHealth = UnitRow->UnitMaxHealth;
	LvlUpComponent->MaxDamage = UnitRow->UnitMaxDamage;

	HealthComponent->MaxHealth = LvlUpComponent->HealthByLvl(UnitLvl);
	HealthComponent->Health = LvlUpComponent->HealthByLvl(UnitLvl);

	Damage = LvlUpComponent->DamageByLvl(UnitLvl);
}

void ABaseUnit::OnHealthChanged(float Health)
{
	HealthWidgetComponent->SetVisibility(true);

	if (HealthChangeTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthChangeTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(HealthChangeTimer,this, &ABaseUnit::HideHealtBar, 2.0f, false);
}

void ABaseUnit::HideHealtBar()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthChangeTimer);
	HealthWidgetComponent->SetVisibility(false);
}

void ABaseUnit::UnitIsDead()
{
	if (UnitController->GetBlackboardComponent())
	{
		UnitController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), true);
	}
	if(DeadAnimMontages.Num() > 0)
	{
		int32 index = rand() % DeadAnimMontages.Num();
		PlayAnimMontage(DeadAnimMontages[index]);
	}
	if (IsEnemy)
	{
		auto MainHero = Cast<AHeroUnit>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MainHero->AddGold(Reward);
		auto GameMode = Cast<AAoddGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->KillEnemy();
	}
	SetLifeSpan(5.0f);
}

void ABaseUnit::AttackTarget(USkeletalMeshComponent* MeshComponent)
{
}

void ABaseUnit::ClearTimerManager()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void ABaseUnit::FindTarget()
{
	if (!IsSomeEnemyInRange()) return;
	int CurrentIndex = 0;
	float MinDistance = 0.0f;
	for (CurrentIndex = 0; CurrentIndex < EnemyList.Num(); CurrentIndex++)
	{
		FVector CharacterLocation = GetActorLocation();
		FVector EnemyLocation = EnemyList[CurrentIndex]->GetActorLocation();
		float CurrentDistanceToEnemy = FVector::Distance(CharacterLocation, EnemyLocation);
		if (CurrentIndex == 0)
		{
			MinDistance = CurrentDistanceToEnemy;
			EnemyIndex = CurrentIndex;
		}
		if (CurrentDistanceToEnemy < MinDistance)
		{
			MinDistance = CurrentDistanceToEnemy;
			EnemyIndex = CurrentIndex;
		}
	}
}

bool ABaseUnit::EnemyListValid()
{
	int32 CurrentIndex = 0;
	for(CurrentIndex = 0; CurrentIndex < EnemyList.Num(); CurrentIndex++)
	{
		auto Target = EnemyList[CurrentIndex];
		if (Target->HealthComponent->IsDead())
		{
			EnemyList.Remove(Target);
		}
	}
	return IsSomeEnemyInRange();
}

void ABaseUnit::EnemyInAttackRange(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HealthComponent->IsDead()) return;

	auto Enemy = Cast<ABaseUnit>(OtherActor);

	if (Enemy->HealthComponent->IsDead()) return;

	EnemyList.Add(Enemy);
	if (UnitController)
	{
		UnitController->GetBlackboardComponent()->SetValueAsBool(TEXT("HasEnemy"), IsSomeEnemyInRange());
	}
}

void ABaseUnit::EnemyOutAttackRange(UPrimitiveComponent* Comp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HealthComponent->IsDead()) return;
	auto  Enemy = Cast<ABaseUnit>(OtherActor);
	EnemyList.Remove(Enemy);
	if (UnitController)
	{
		UnitController->GetBlackboardComponent()->SetValueAsBool(TEXT("HasEnemy"), IsSomeEnemyInRange());
	}
}

bool ABaseUnit::IsSomeEnemyInRange()
{
	if (EnemyList.Num() == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ABaseUnit::InitAnimations()
{
	if (AttackAnimMontages.Num() <= 0) return;
	for (auto AttackAM : AttackAnimMontages)
	{
		const auto NotifyEvents = AttackAM->Notifies;
		for (auto NotifyEvent : NotifyEvents)
		{
			auto AttackFinishedNotify = Cast<UAttackFinishedNotify>(NotifyEvent.Notify);
			if (AttackFinishedNotify)
			{
				AttackFinishedNotify->OnNotified.AddUObject(this, &ABaseUnit::OnAttackFinished);
				break;
			}
		}
		for (auto NotifyEvent : NotifyEvents)
		{
			auto DoDamageNotify = Cast<UDoDamageNotify>(NotifyEvent.Notify);
			if (DoDamageNotify)
			{
				DoDamageNotify->OnNotified.AddUObject(this, &ABaseUnit::AttackTarget);
				break;
			}
		}
	}
}

void ABaseUnit::OnAttackFinished(USkeletalMeshComponent* MeshComponent)
{
	if (GetMesh() == MeshComponent)
	{
		UnitController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), false);
	}
}