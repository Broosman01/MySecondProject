// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UpgradeCardWidget.h"
#include "Components/Button.h"
#include "Units/HeroUnit.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/UnitsLvlSaveGame.h"
#include "Components/LvlUpComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UUpgradeCardWidget::NativeConstruct()
{
	if (UpgradeButton)
	{
		UpgradeButton->OnClicked.AddDynamic(this, &UUpgradeCardWidget::UpgradeUnit);
	}
}

void UUpgradeCardWidget::UpgradeUnit()
{
	OnUpgradeLvl.Broadcast(UnitCardType);
}

void UUpgradeCardWidget::SetupCard(EUnitType UnitType, int32 Lvl)
{
	UnitCardType = UnitType;
	LvlUpComponent = LvlUpComponentClass.GetDefaultObject();
	FString UnitTablePath(TEXT("DataTable'/Game/DataTables/Units_DT.Units_DT'"));
	UDataTable* UnitTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *UnitTablePath));
	if (UnitTableObject)
	{
		UnitRow = nullptr;
		FString EnumName = UEnum::GetValueAsString(UnitType);
		FString CurrentName = EnumName.Replace(TEXT("EUnitType::EUT_"), TEXT(""), ESearchCase::IgnoreCase);

		UnitRow = UnitTableObject->FindRow<FUnitTable>(FName(*CurrentName), TEXT(""));
		if (UnitRow)
		{
			UpdateCard(Lvl);
		}
	}
}

void UUpgradeCardWidget::UpdateCard(int32 Lvl)
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
		if (Lvl >= LvlUpComponent->MaxLvl)
		{
			UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
			UnitLvlText->SetText(FText::FromString(TEXT("Max")));
			return;
		}
		UpgradeCostText->SetText(FText::AsNumber(LvlUpComponent->GoldForUpByLvl(Lvl)));
		UnitLvlText->SetText(FText::AsNumber(Lvl));
	}
}