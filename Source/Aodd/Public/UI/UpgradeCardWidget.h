// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Units/BaseUnit.h"
#include "UpgradeCardWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpgradeLvl, EUnitType)

class UButton;
class UTextBlock;
class ULvlUpComponent;
class AHeroUnit;
class UImage;
class UUnitsLvlSaveGame;

UCLASS()
class AODD_API UUpgradeCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EUnitType UnitCardType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ULvlUpComponent> LvlUpComponentClass;

	ULvlUpComponent* LvlUpComponent;

	FOnUpgradeLvl OnUpgradeLvl;

	void SetupCard(EUnitType UnitType, int32 Lvl);
	void UpdateCard(int32 Lvl);
protected:
	virtual void NativeConstruct() override;

	FUnitTable* UnitRow;

	UPROPERTY(meta = (BindWidget))
	UButton* UpgradeButton;

	UPROPERTY(meta = (BindWidget))
	UImage* UnitImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UnitNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UnitLvlText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UnitHealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UnitDamageText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UpgradeCostText;
private:
	UFUNCTION()
	void UpgradeUnit();
};
