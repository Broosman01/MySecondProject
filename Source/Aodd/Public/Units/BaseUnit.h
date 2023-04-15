// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Animation/AnimBlueprint.h"
#include "BaseUnit.generated.h"

class UMyHealthComponent;
class ULvlUpComponent;
class UAnimMontage;
class UBoxComponent;
class UBehaviorTree;
class UWidgetComponent;
class UUnitsLvlSaveGame;


UENUM(BlueprintType)
enum class EUnitType : uint8
{
	EUT_Worker UMETA(DisplayName = "Worker"),
	EUT_AxmanLight UMETA(DisplayName = "AxmanLight"),
	EUT_SwordmanLight UMETA(DisplayName = "SwordmanLight"),
	EUT_Bowman UMETA(DisplayName = "Bowman"),
	EUT_Spearman UMETA(DisplayName = "Spearman"),
	EUT_Axman UMETA(DisplayName = "Axman"),
	EUT_Swordman UMETA(DisplayName = "Swordman"),
	EUT_ElseUnit UMETA(DisplayName = "ElseUnit"),
	EUT_MAX UMETA(DisplayName = "MAX")
};

USTRUCT(BlueprintType)
struct FUnitTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEnemyUnit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ABaseUnit>> UnitClassList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* UnitIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UnitCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UnitMaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UnitMaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UnitMaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UnitMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* UnitMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> AttackAnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> DeadAnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* UnitAnim_BP;
};

UCLASS()
class AODD_API ABaseUnit : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseUnit();

	virtual void AttackTarget(USkeletalMeshComponent* MeshComponent);
	virtual void ClearTimerManager();
	bool EnemyListValid();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<UAnimMontage*> AttackAnimMontages;

	class UDataTable* UnitTableObject;

	TArray<ABaseUnit*> EnemyList;
	int32 EnemyIndex;
	ABaseUnit* EnemyTarget;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UMyHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULvlUpComponent* LvlUpComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitType", meta = (AllowPrivateAccess = "true"))
	bool IsEnemy = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool SplashAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 0.0f;

	UFUNCTION()
	virtual void EnemyInAttackRange(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EnemyOutAttackRange(UPrimitiveComponent* Comp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void FindTarget();

	bool IsSomeEnemyInRange();
	void SetupAttackRange(float Range);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<UAnimMontage*> DeadAnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitType", meta = (AllowPrivateAccess = "true"))
	EUnitType UnitType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UnitType", meta = (AllowPrivateAccess = "true"))
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UnitType", meta = (AllowPrivateAccess = "true"))
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UnitType", meta = (AllowPrivateAccess = "true"))
	int32 Cost = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UnitType", meta = (AllowPrivateAccess = "true"))
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UnitType", meta = (AllowPrivateAccess = "true"))
	int32 Reward = 0;

	FVector EndPoint;

	FTimerHandle AttackTimerHandle;
	FTimerHandle HealthChangeTimer;

	FUnitTable* UnitRow;
	UUnitsLvlSaveGame* UpSaveGame;

	class ABaseUnitAIController* UnitController;
private:
	void LoadUnitLvls();
	void SetupDamageAndHealth();
	virtual void UnitIsDead();
	void OnHealthChanged(float Health);
	void HideHealtBar();

	void InitAnimations();
	void OnAttackFinished(USkeletalMeshComponent* MeshComponent);

	virtual void OnConstruction(const FTransform& Transform) override;
};
