// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitHealthWidget.h"
#include "Units/HeroUnit.h"
#include "Curves/CurveLinearColor.h"
#include "Components/MyHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Units/BaseUnit.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUnitHealthWidget::NativeConstruct()
{
	if (PlayerOwner)
	{
		PlayerOwner->HealthComponent->OnHealthChanged.AddUObject(this, &UUnitHealthWidget::ChangeHealth);
		ChangeHealth(PlayerOwner->HealthComponent->GetHealth());
	}
}

float UUnitHealthWidget::GetHealthPercent() const
{
	return PlayerOwner->HealthComponent->GetHealthPercent();
}

void UUnitHealthWidget::ChangeHealth(float Health)
{
	HealthBar->SetPercent(GetHealthPercent());
	if (HealthColorCurve)
	{
		HealthBar->SetFillColorAndOpacity(HealthColorCurve->GetLinearColorValue(GetHealthPercent()));
	}
	MaxHealthText->SetText(FText::AsNumber(PlayerOwner->HealthComponent->GetMaxHealth()));
	HealthText->SetText(FText::AsNumber(Health));
}