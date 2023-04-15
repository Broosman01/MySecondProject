// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "SaveGame/UnitsLvlSaveGame.h"

void UMenuWidget::NativeConstruct()
{
	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGame);
	}
	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnRestartGame);
	}
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::EndGame);
	}
}

void UMenuWidget::OnStartGame()
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

void UMenuWidget::OnRestartGame()
{
	if (RestartGameWidgetClass)
	{
		const auto RestartGameWidget = CreateWidget<UUserWidget>(GetWorld(), RestartGameWidgetClass);
		if (RestartGameWidget)
		{
			RestartGameWidget->AddToViewport();
		}
	}
	RemoveFromParent();
}

void UMenuWidget::EndGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
