// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UpgradeWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/LvlUpComponent.h"
#include "Components/TextBlock.h"
#include "UI/UpgradeCardWidget.h"
#include "SaveGame/UnitsLvlSaveGame.h"
#include "SaveGame/GoldSaveGame.h"

DEFINE_LOG_CATEGORY_STATIC(UpgradeUnitLog, All, All);

void UUpgradeWidget::NativeConstruct()
{
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UUpgradeWidget::Back);
	}
	LoadSaveGame();
	SetupWidget();
}

void UUpgradeWidget::Back()
{
	SaveUnitLvls();
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

void UUpgradeWidget::SetupWidget()
{
	if (!UpgradeCardsBox || UnitsAndLvls.Num() == 0) return;
	UpgradeCardsBox->ClearChildren();
	UpgradeCards.Empty();
	LvlUpComponent = LvlUpComponentClass.GetDefaultObject();
	GoldText->SetText(FText::AsNumber(Gold));

	TArray<EUnitType> Keys;
	UnitsAndLvls.GenerateKeyArray(Keys);

	for (int32 i = 0; i < UnitsAndLvls.Num(); i++)
	{
		const auto UpgradeCard = CreateWidget<UUpgradeCardWidget>(GetWorld(), UpgradeCardWidgetClass);
		UpgradeCard->OnUpgradeLvl.AddUObject(this, &UUpgradeWidget::UpgradeUnit);
		UpgradeCard->SetupCard(Keys[i], UnitsAndLvls.FindRef(Keys[i]));
		UpgradeCards.AddUnique(UpgradeCard);

		UpgradeCardsBox->AddChildToVerticalBox(UpgradeCard);
	}
}

void UUpgradeWidget::UpgradeUnit(EUnitType UnitForUp)
{
	TArray<EUnitType> Keys;
	UnitsAndLvls.GenerateKeyArray(Keys);

	TArray<int32> Values;
	UnitsAndLvls.GenerateValueArray(Values);

	int32 Index = Keys.Find(UnitForUp);

	int32 UnitLvl = Values[Index];
	
	if (Gold < LvlUpComponent->GoldForUpByLvl(UnitLvl)|| UnitLvl >= LvlUpComponent->MaxLvl) return;

	Gold -= LvlUpComponent->GoldForUpByLvl(UnitLvl);
	GoldText->SetText(FText::AsNumber(Gold));
	UnitLvl++;

	TMap<EUnitType, int32> NewUnitLvls;

	for (int32 i = 0; i < UnitsAndLvls.Num(); i++)
	{
		if (i != Index)
		{
			NewUnitLvls.Add(Keys[i], Values[i]);
		}
		else
		{
			NewUnitLvls.Add(Keys[i], UnitLvl);
		}
	}
	UnitsAndLvls = NewUnitLvls;
	UpgradeCards[Index]->UpdateCard(UnitLvl);
}

void UUpgradeWidget::LoadSaveGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("UnitLvls"), 0)) return;

	UnitsLvlSGRef = Cast<UUnitsLvlSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("UnitLvls"), 0));
	if (UnitsLvlSGRef->UnitsLvl.Num() == 0) return;
	UnitsAndLvls = UnitsLvlSGRef->UnitsLvl;

	if (!UGameplayStatics::DoesSaveGameExist(TEXT("Gold"), 0)) return;
	GoldSGRef = Cast<UGoldSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Gold"), 0));
	Gold = GoldSGRef->HeroGold;
}

void UUpgradeWidget::SaveUnitLvls()
{
	UnitsLvlSGRef = Cast<UUnitsLvlSaveGame>(UGameplayStatics::CreateSaveGameObject(UUnitsLvlSaveGame::StaticClass()));
	GoldSGRef = Cast<UGoldSaveGame>(UGameplayStatics::CreateSaveGameObject(UGoldSaveGame::StaticClass()));

	UnitsLvlSGRef->UnitsLvl = UnitsAndLvls;
	GoldSGRef->HeroGold = Gold;

	UGameplayStatics::SaveGameToSlot(UnitsLvlSGRef, TEXT("UnitLvls"), 0);
	UGameplayStatics::SaveGameToSlot(GoldSGRef, TEXT("Gold"), 0);
}

