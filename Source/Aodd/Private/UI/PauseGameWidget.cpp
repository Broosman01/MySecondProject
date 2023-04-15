// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseGameWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"


void UPauseGameWidget::NativeConstruct()
{
	if (!ClearPauseButton) return;
	ClearPauseButton->OnClicked.AddDynamic(this, &UPauseGameWidget::ClearPause);
	QuitButton->OnClicked.AddDynamic(this, &UPauseGameWidget::QuitLvl);
}

void UPauseGameWidget::ClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();
}

void UPauseGameWidget::QuitLvl()
{
	const FName MenuLvlName = "MainMenuLvl";
	UGameplayStatics::OpenLevel(this, MenuLvlName);
}