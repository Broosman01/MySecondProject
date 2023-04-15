// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectUnitWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "UI/UnitCardWidget.h"
#include "SaveGame/SelectedUnitsSaveGame.h"
#include "SaveGame/UnitsLvlSaveGame.h"

DEFINE_LOG_CATEGORY_STATIC(SelectUnitLog, All, All);

void USelectUnitWidget::NativeConstruct()
{
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USelectUnitWidget::Back);
	}

	if (StartLvlButton)
	{
		StartLvlButton->OnClicked.AddDynamic(this, &USelectUnitWidget::Start);
	}
	SelectedUnits.Empty();
	InitCards();
}

void USelectUnitWidget::Back()
{
	if (SelectLvlWidgetClass)
	{
		const auto SelectLvlWidget = CreateWidget<UUserWidget>(GetWorld(), SelectLvlWidgetClass);
		if (SelectLvlWidget)
		{
			SelectLvlWidget->AddToViewport();
		}
	}
	RemoveFromParent();
}

void USelectUnitWidget::Start()
{
	if (SelectedUnits.Num() != MaxUnitsCount) return;

	USelectedUnitsSaveGame* SaveGameRef = Cast<USelectedUnitsSaveGame>(UGameplayStatics::CreateSaveGameObject(USelectedUnitsSaveGame::StaticClass()));

	TArray<EUnitType>Keys;
	SelectedUnits.GenerateKeyArray(Keys);
	SaveGameRef->SelectedUnits = Keys;
	UGameplayStatics::SaveGameToSlot(SaveGameRef, TEXT("SelectedUnits"), 0);

	const FName StartUpLvlName = "TestLvl";
	UGameplayStatics::OpenLevel(this, StartUpLvlName);
}

void USelectUnitWidget::InitCards()
{
	if (!UnitCardsPanel || AllUnits.Num() == 0 || !UnitCardsWidgetClass) return;
	LoadUnitLvls();
	
	UnitCardsPanel->ClearChildren();

	for (int32 CardNumber = 0; CardNumber < AllUnits.Num(); CardNumber++)
	{
		const auto CardWidget = CreateWidget<UUnitCardWidget>(GetWorld(), UnitCardsWidgetClass);

		if (!CardWidget) continue;
		CardWidget->SetupCard(AllUnits[CardNumber], GetUnitLvls(AllUnits[CardNumber]));
		CardWidget->OnSelectUnit.AddUObject(this, &USelectUnitWidget::AddSelectedUnit);

		int32 MaxColumnNumber = 3;
		int32 Column = CardNumber % MaxColumnNumber;
		int32 Row = CardNumber / MaxColumnNumber;

		UnitCardsPanel->AddChildToUniformGrid(CardWidget, Row, Column);
	}
}

void USelectUnitWidget::AddSelectedUnit(EUnitType SelectedUnit)
{
	if (!SelectedUnitCardsPanel || SelectedUnits.Contains(SelectedUnit) ||SelectedUnits.Num() >= MaxUnitsCount) return;

	int32 CardNumber = 0;
	if (SelectedUnits.Num() > 0)
	{
		CardNumber = SelectedUnits.Num();
	}
	UUnitCardWidget* SelectedCardWidget = CreateWidget<UUnitCardWidget>(GetWorld(), UnitCardsWidgetClass);

	if (!SelectedCardWidget)return;

	SelectedUnits.Add(SelectedUnit, SelectedCardWidget);
	SelectedCardWidget->SelectedCard = true;
	SelectedCardWidget->SetupCard(SelectedUnit,GetUnitLvls(SelectedUnit));
	SelectedCardWidget->OnDeSelectUnit.AddUObject(this, &USelectUnitWidget::RemoveSelectedUnit);

	int32 MaxColumnNumber = 5;
	int32 Column = CardNumber % MaxColumnNumber;
	int32 Row = CardNumber / MaxColumnNumber;

	SelectedUnitCardsPanel->AddChildToUniformGrid(SelectedCardWidget, Row, Column);
}

void USelectUnitWidget::RemoveSelectedUnit(EUnitType SelectedUnit)
{
	TArray<EUnitType> Keys;
	SelectedUnits.GenerateKeyArray(Keys);
	int32 i = Keys.Find(SelectedUnit);
	Keys.Remove(SelectedUnit);

	SelectedUnitCardsPanel->ClearChildren();
	SelectedUnits.Empty();

	for (int32 CardNumber = 0; CardNumber < Keys.Num(); CardNumber++)
	{
		UUnitCardWidget* SelectedCardWidget = CreateWidget<UUnitCardWidget>(GetWorld(), UnitCardsWidgetClass);

		if (!SelectedCardWidget)return;

		int32 MaxColumnNumber = 5;
		int32 Column = CardNumber % MaxColumnNumber;
		int32 Row = CardNumber / MaxColumnNumber;

		SelectedUnitCardsPanel->AddChildToUniformGrid(SelectedCardWidget, Row, Column);

		SelectedUnits.Add(Keys[CardNumber], SelectedCardWidget);
		SelectedCardWidget->SelectedCard = true;
		SelectedCardWidget->SetupCard(Keys[CardNumber], GetUnitLvls(Keys[CardNumber]));
		SelectedCardWidget->OnDeSelectUnit.AddUObject(this, &USelectUnitWidget::RemoveSelectedUnit);
	}
}

void USelectUnitWidget::LoadUnitLvls()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("UnitLvls"), 0))
	{
		UpSaveGame = Cast<UUnitsLvlSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("UnitLvls"), 0));
		AllUnitsAndLvls = UpSaveGame->UnitsLvl;
	}
}

int32 USelectUnitWidget::GetUnitLvls(EUnitType Unit)
{
	int32 Lvl = 1;
	FString EnumName = UEnum::GetValueAsString(Unit);

	if (!UpSaveGame)
	{
		return Lvl;
	}

	TArray<EUnitType>Keys;
	UpSaveGame->UnitsLvl.GenerateKeyArray(Keys);

	Lvl = AllUnitsAndLvls.FindRef(Unit);

	UE_LOG(SelectUnitLog, Error, TEXT("%s lvl:%d"), *EnumName, Lvl);
	return Lvl;
}