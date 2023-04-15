// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RestartGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"


void URestartGameWidget::NativeConstruct()
{
	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddDynamic(this, &URestartGameWidget::RestartGame);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &URestartGameWidget::Back);
	}
}


void URestartGameWidget::RestartGame()
{
	UGameplayStatics::DeleteGameInSlot(TEXT("UnitLvls"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("CompletedLvls"), 0);
	Back();
}

void URestartGameWidget::Back()
{
	if (!MainMenuWidgetClass) return;
	const auto MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
		RemoveFromParent();
	}
}
