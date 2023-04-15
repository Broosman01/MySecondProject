// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "Units/BaseUnit.h"
#include "Components/Image.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class AHeroUnit;
class ABaseUnit;

enum class EUnitType : uint8;

UCLASS()
class AODD_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<EUnitType> Units;

	AHeroUnit* PlayerOwner;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MaxHealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditDefaultsOnly)
	UCurveLinearColor* HealthColorCurve;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentGoldText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentProducerLvlText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentMetalText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentMaxMetalText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MetalForUpText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* MetalBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitCostText01;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitCostText02;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitCostText03;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitCostText04;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitCostText05;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* UnitImage01;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* UnitImage02;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* UnitImage03;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* UnitImage04;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* UnitImage05;

	virtual void NativeConstruct() override;

private:
	class UDataTable* UnitTableObject;

	void ChangeHealth(float Health);

	UFUNCTION(BlueprintCallable)
	void ChangeMetal();
	void ChangeStats();

	UFUNCTION(BlueprintCallable)
	void ChangeUnitIconsAndText();

	UFUNCTION()
	void ChangeCurrentGold(int32 CurrentGold);
};
