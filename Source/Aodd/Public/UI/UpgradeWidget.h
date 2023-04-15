// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Units/BaseUnit.h"
#include "UpgradeWidget.generated.h"

class UButton;
class UTextBlock;
class ULvlUpComponent;
class UVerticalBox;
class UUnitsLvlSaveGame;
class UUpgradeCardWidget;

UCLASS()
class AODD_API UUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* UpgradeCardsBox;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ULvlUpComponent> LvlUpComponentClass;

	ULvlUpComponent* LvlUpComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SelectLvlWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> UpgradeCardWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TMap<EUnitType, int32> UnitsAndLvls;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Money")
	int32 Gold = 0;

	UUnitsLvlSaveGame* UnitsLvlSGRef;
	UGoldSaveGame* GoldSGRef;

	TArray<UUpgradeCardWidget*> UpgradeCards;


	virtual void NativeConstruct() override;
private:
	void SetupWidget();
	void UpgradeUnit(EUnitType UnitForUp);

	void LoadSaveGame();
	void SaveUnitLvls();

	UFUNCTION()
	void Back();
};
