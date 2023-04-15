// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WinMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/UnitsLvlSaveGame.h"
#include "SaveGame/GoldSaveGame.h"
#include "Units/HeroUnit.h"

void UWinMenuWidget::NativeConstruct()
{
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UWinMenuWidget::Quit);
	}
}

void UWinMenuWidget::Quit()
{
	SaveGold();
	const FName MenuLvlName = "MainMenuLvl";
	UGameplayStatics::OpenLevel(this, MenuLvlName);
}

void UWinMenuWidget::SaveGold()
{
	auto GoldSGRef = Cast<UGoldSaveGame>(UGameplayStatics::CreateSaveGameObject(UGoldSaveGame::StaticClass()));
	if (!GoldSGRef) return;

	auto MainHero = Cast<AHeroUnit>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!MainHero) return;

	GoldSGRef->HeroGold = MainHero->Gold;
	UGameplayStatics::SaveGameToSlot(GoldSGRef, TEXT("Gold"), 0);
}