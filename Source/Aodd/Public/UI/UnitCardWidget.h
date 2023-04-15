// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Units/BaseUnit.h"
#include "UnitCardWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectUnit, EUnitType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeSelectUnit, EUnitType)

class UTextBlock;
class ULvlUpComponent;

UCLASS()
class AODD_API UUnitCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupCard(EUnitType UnitType, int32 Lvl);

	bool SelectedCard = false;
	EUnitType SelectedUnitType;

	FOnSelectUnit OnSelectUnit;
	FOnDeSelectUnit OnDeSelectUnit;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ULvlUpComponent> LvlUpComponentClass;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectUnitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SelectDeselectText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* UnitImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitLvlText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitHealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnitDamageText;

	FUnitTable* UnitRow;
	ULvlUpComponent* LvlUpComponent;

	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void SelectDeSelectUnit();

	void UpdateStats(int32 Lvl);
};
