// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Units/BaseUnit.h"
#include "SelectUnitWidget.generated.h"

class UButton;
class UUniformGridPanel;
class UUnitCardWidget;
class UUnitsLvlSaveGame;

UCLASS()
class AODD_API USelectUnitWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SelectLvlWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> UnitCardsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EUnitType, UWidget*> SelectedUnits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<EUnitType> AllUnits;

	TMap<EUnitType, int32> AllUnitsAndLvls;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* UnitCardsPanel;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* SelectedUnitCardsPanel;

	UPROPERTY(meta = (BindWidget))
	UButton* StartLvlButton;

	UUnitsLvlSaveGame* UpSaveGame;

	virtual void NativeConstruct() override;
private:
	int32 MaxUnitsCount = 5;

	UPROPERTY()
	TArray<UUserWidget*>UnitCardsWidgets;

	UFUNCTION()
	void Back();

	UFUNCTION()
	void Start();

	UFUNCTION()
	void AddSelectedUnit(EUnitType SelectedUnit);

	UFUNCTION()
	void RemoveSelectedUnit(EUnitType SelectedUnit);

	void InitCards();
	void LoadUnitLvls();
	int32 GetUnitLvls(EUnitType Unit);
};
