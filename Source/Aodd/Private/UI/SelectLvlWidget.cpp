// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectLvlWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SaveGame/SelectedLvlSaveGame.h"
#include "SaveGame/CompleteLvlsSaveGame.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void USelectLvlWidget::NativeConstruct()
{
	if (StartLvlButton)
	{
		StartLvlButton->OnClicked.AddDynamic(this, &USelectLvlWidget::OnStartLvl);
	}
	if (UpgradesButton)
	{
		UpgradesButton->OnClicked.AddDynamic(this, &USelectLvlWidget::Upgrades);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USelectLvlWidget::Back);
	}
	if (NextLvlButton)
	{
		NextLvlButton->OnClicked.AddDynamic(this, &USelectLvlWidget::NextLvl);
	}
	if (PreviousLvlButton)
	{
		PreviousLvlButton->OnClicked.AddDynamic(this, &USelectLvlWidget::PreviousLvl);
	}
	CurrentLvl = MinLvl;
	CurrentLvlText->SetText(FText::AsNumber(CurrentLvl));

	LoadCompletedLvls();
}

void USelectLvlWidget::OnStartLvl()
{
	if (SelectUnitWidgetClass)
	{
		const auto SelectUnitWidget = CreateWidget<UUserWidget>(GetWorld(), SelectUnitWidgetClass);
		if (SelectUnitWidget)
		{
			SelectUnitWidget->AddToViewport();
		}
	}
	SaveChoosenLvl();
	RemoveFromParent();
}

void USelectLvlWidget::Upgrades()
{
	if (UpgradesWidgetClass)
	{
		const auto UpgradesWidget = CreateWidget<UUserWidget>(GetWorld(), UpgradesWidgetClass);
		if (UpgradesWidget)
		{
			UpgradesWidget->AddToViewport();
		}
	}
	RemoveFromParent();
}

void USelectLvlWidget::Back()
{
	if (MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
	RemoveFromParent();
}

void USelectLvlWidget::NextLvl()
{
	if (CurrentLvl == CurrentMaxLvl)
	{
		CurrentLvl = MinLvl;
		CurrentLvlText->SetText(FText::AsNumber(CurrentLvl));
	}
	else
	{
		CurrentLvl++;
		CurrentLvlText->SetText(FText::AsNumber(CurrentLvl));
	}
}

void USelectLvlWidget::PreviousLvl()
{
	if (CurrentLvl == MinLvl)
	{
		CurrentLvl = CurrentMaxLvl;
		CurrentLvlText->SetText(FText::AsNumber(CurrentLvl));
	}
	else
	{
		CurrentLvl--;
		CurrentLvlText->SetText(FText::AsNumber(CurrentLvl));
	}
}

void USelectLvlWidget::SaveChoosenLvl()
{
	auto SaveGameRef = Cast<USelectedLvlSaveGame>(UGameplayStatics::CreateSaveGameObject(USelectedLvlSaveGame::StaticClass()));
	SaveGameRef->CurrentLvl = CurrentLvl;

	UGameplayStatics::SaveGameToSlot(SaveGameRef, TEXT("SelectedLvl"), 0);
}

void USelectLvlWidget::LoadCompletedLvls()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("CompletedLvls"), 0))
	{
		CompletedLvlsSG = Cast<UCompleteLvlsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("CompletedLvls"), 0));
		CurrentMaxLvl = CompletedLvlsSG->CompletedLvls + 1;
		CurrentMaxLvl = FMath::Min(CurrentMaxLvl, MaxLvl);
	}
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("CompletedLvls"), 0))
	{
		CurrentMaxLvl = MinLvl;
	}
}