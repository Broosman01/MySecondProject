// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"
#include "Units/HeroUnit.h"
#include "Curves/CurveLinearColor.h"
#include "Components/MyHealthComponent.h"
#include "Components/UnitSpawnerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Units/BaseUnit.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

void UPlayerHUDWidget::NativeConstruct()
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return;

	PlayerOwner = Cast<AHeroUnit>(Player);
	if (!PlayerOwner) return;

	PlayerOwner->HealthComponent->OnHealthChanged.AddUObject(this, &UPlayerHUDWidget::ChangeHealth);
	PlayerOwner->UnitSpawnerComponent->OnMetalChanged.AddUObject(this, &UPlayerHUDWidget::ChangeMetal);
	PlayerOwner->UnitSpawnerComponent->OnStatsChanged.AddUObject(this, &UPlayerHUDWidget::ChangeStats);
	PlayerOwner->OnChangeIcons.AddUObject(this, &UPlayerHUDWidget::ChangeUnitIconsAndText);
	PlayerOwner->OnChangeGold.AddUObject(this, &UPlayerHUDWidget::ChangeCurrentGold);

	FString UnitTablePath(TEXT("DataTable'/Game/DataTables/Units_DT.Units_DT'"));
	UnitTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *UnitTablePath));
}

float UPlayerHUDWidget::GetHealthPercent() const
{
	return PlayerOwner->HealthComponent->GetHealthPercent();
}

void UPlayerHUDWidget::ChangeHealth(float Health)
{
	HealthBar->SetPercent(GetHealthPercent());

	if (HealthColorCurve)
	{
		HealthBar->SetFillColorAndOpacity(HealthColorCurve->GetLinearColorValue(GetHealthPercent()));
	}

	MaxHealthText->SetText(FText::AsNumber(PlayerOwner->HealthComponent->GetMaxHealth()));
	HealthText->SetText(FText::AsNumber(Health));
}

void UPlayerHUDWidget::ChangeMetal()
{
	int32 CurrentMetal = PlayerOwner->UnitSpawnerComponent->CurrentMetal;
	CurrentMetalText->SetText(FText::AsNumber(CurrentMetal));
	MetalBar->SetPercent(PlayerOwner->UnitSpawnerComponent->GetMetalPercent());
}

void UPlayerHUDWidget::ChangeStats()
{
	int32 CurrentProducerLvl = PlayerOwner->UnitSpawnerComponent->CurrentProducerLvl;
	int32 CurrentMaxMetal = PlayerOwner->UnitSpawnerComponent->CurrentMaxMetal;
	int32 CurrentMetal = PlayerOwner->UnitSpawnerComponent->CurrentMetal;
	int32 MetalForUp = PlayerOwner->UnitSpawnerComponent->MetalForUp;

	CurrentProducerLvlText->SetText(FText::AsNumber(CurrentProducerLvl));
	CurrentMaxMetalText->SetText(FText::AsNumber(CurrentMaxMetal));
	CurrentMetalText->SetText(FText::AsNumber(CurrentMetal));
	MetalForUpText->SetText(FText::AsNumber(MetalForUp));
	if (PlayerOwner->UnitSpawnerComponent->CurrentProducerLvl >= PlayerOwner->UnitSpawnerComponent->MaxProducerLvl)
	{
		MetalForUpText->SetText(FText::FromString(TEXT("MaxLvl")));
	}

	MetalBar->SetPercent(PlayerOwner->UnitSpawnerComponent->GetMetalPercent());
}

void UPlayerHUDWidget::ChangeCurrentGold(int32 CurrentGold)
{
	CurrentGoldText->SetText(FText::AsNumber(CurrentGold));
}

void UPlayerHUDWidget::ChangeUnitIconsAndText()
{
	TArray<UTextBlock*>TextArray = {
		UnitCostText01,
		UnitCostText02,
		UnitCostText03,
		UnitCostText04,
		UnitCostText05
	};

	TArray<UImage*>ImageArray = {
		UnitImage01,
		UnitImage02,
		UnitImage03,
		UnitImage04,
		UnitImage05
	};
	Units = PlayerOwner->ChoosenUnits;
	if (Units.Num() == 0) return;

	const auto UnitDataTable = PlayerOwner->UnitTableObject;
	TArray<FName> RowNames = PlayerOwner->UnitTableObject->GetRowNames();

	for (int i = 0; i < TextArray.Num(); i++)
	{
		FString EnumName = UEnum::GetValueAsString(Units[i]);
		FString CurrentName = EnumName.Replace(TEXT("EUnitType::EUT_"), TEXT(""), ESearchCase::IgnoreCase);

		FUnitTable* OutRow = PlayerOwner->UnitTableObject->FindRow<FUnitTable>(FName(*CurrentName), TEXT(""));
		if (!OutRow) return;
		int32 Cost = OutRow->UnitCost;

		TextArray[i]->SetText(FText::AsNumber(Cost));
		ImageArray[i]->SetBrushFromTexture(OutRow->UnitIcon);
	}
}
