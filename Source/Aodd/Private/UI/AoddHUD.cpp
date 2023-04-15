// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AoddHUD.h"
#include "Engine/Canvas.h"
#include "Aodd/AoddGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/PauseGameWidget.h"
#include "UI/WinMenuWidget.h"
#include "Units/HeroUnit.h"
#include "Kismet/GameplayStatics.h"

void AAoddHUD::BeginPlay()
{
	Super::BeginPlay();
	PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayeHudWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}

	const auto GameMode = Cast<AAoddGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnPauseSet.AddUObject(this, &AAoddHUD::SetPauseWidget);
		GameMode->OnPauseReset.AddUObject(this, &AAoddHUD::ResetPauseWidget);
		GameMode->OnWinMenuSet.AddUObject(this, &AAoddHUD::SetWinMenuWidget);
		GameMode->OnLoseMenuSet.AddUObject(this, &AAoddHUD::SetLoseMenuWidget);
	}
}

void AAoddHUD::SetPauseWidget()
{
	PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
	if (PauseWidgetClass)
	{
		PauseWidget->AddToViewport();
	}
}

void AAoddHUD::ResetPauseWidget()
{
	if (!PauseWidget) return;

	PauseWidget->RemoveFromParent();
}

void AAoddHUD::SetWinMenuWidget()
{
	WinMenuWidget = CreateWidget<UUserWidget>(GetWorld(), WinMenuWidgetClass);
	if (WinMenuWidget)
	{
		WinMenuWidget->AddToViewport();
	}
}

void AAoddHUD::SetLoseMenuWidget()
{
	LoseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), LoseMenuWidgetClass);
	if (LoseMenuWidget)
	{
		LoseMenuWidget->AddToViewport();
	}
}


