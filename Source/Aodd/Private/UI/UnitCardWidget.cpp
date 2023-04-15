// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitCardWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/LvlUpComponent.h"

DEFINE_LOG_CATEGORY_STATIC(SelectUnitCardLog, All, All);

void UUnitCardWidget::NativeConstruct()
{
	if (SelectUnitButton)
	{
		SelectUnitButton->OnClicked.AddDynamic(this, &UUnitCardWidget::SelectDeSelectUnit);
	}
}

void UUnitCardWidget::SetupCard(EUnitType UnitType, int32 Lvl)
{
	SelectedUnitType = UnitType;
	LvlUpComponent = LvlUpComponentClass.GetDefaultObject();
	FString UnitTablePath(TEXT("DataTable'/Game/DataTables/Units_DT.Units_DT'"));
	UDataTable* UnitTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *UnitTablePath));
	
	if (UnitTableObject)
	{
		UnitRow = nullptr;
		FString EnumName = UEnum::GetValueAsString(UnitType);
		FString CurrentName = EnumName.Replace(TEXT("EUnitType::EUT_"), TEXT(""), ESearchCase::IgnoreCase);

		UnitRow = UnitTableObject->FindRow<FUnitTable>(FName(*CurrentName), TEXT(""));
		UpdateStats(Lvl);
	}
}

void UUnitCardWidget::SelectDeSelectUnit()
{
	if (SelectedCard)
	{
		OnDeSelectUnit.Broadcast(SelectedUnitType);
		return;
	}
	OnSelectUnit.Broadcast(SelectedUnitType);
}

void UUnitCardWidget::UpdateStats(int32 Lvl)
{
	if (UnitRow)
	{
		UnitImage->SetBrushFromTexture(UnitRow->UnitIcon);
		UnitNameText->SetText(FText::FromName(UnitRow->UnitName));
		UnitLvlText->SetText(FText::AsNumber(Lvl));
		if (LvlUpComponent)
		{
			LvlUpComponent->MaxHealth = UnitRow->UnitMaxHealth;
			LvlUpComponent->MaxDamage = UnitRow->UnitMaxDamage;
			UnitHealthText->SetText(FText::AsNumber(LvlUpComponent->HealthByLvl(Lvl)));
			UnitDamageText->SetText(FText::AsNumber(LvlUpComponent->DamageByLvl(Lvl)));
		}
		if (SelectedCard)
		{
			SelectDeselectText->SetText(FText::FromString(TEXT("Deselect Unit")));
			return;
		}
		SelectDeselectText->SetText(FText::FromString(TEXT("Select Unit")));
	}
}

