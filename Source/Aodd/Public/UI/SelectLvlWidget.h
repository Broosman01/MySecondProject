// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectLvlWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class AODD_API USelectLvlWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentLvl = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentMaxLvl = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MinLvl = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxLvl = 25;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartLvlButton;

	UPROPERTY(meta = (BindWidget))
	UButton* UpgradesButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NextLvlButton;

	UPROPERTY(meta = (BindWidget))
	UButton* PreviousLvlButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentLvlText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SelectUnitWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> UpgradesWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UCompleteLvlsSaveGame* CompletedLvlsSG;
	
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnStartLvl();

	UFUNCTION()
	void Upgrades();

	UFUNCTION()
	void Back();

	UFUNCTION()
	void NextLvl();

	UFUNCTION()
	void PreviousLvl();

	void LoadCompletedLvls();
	void SaveChoosenLvl();
};
