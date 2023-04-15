// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitHealthWidget.generated.h"

class UTextBlock;
class UProgressBar;
class ABaseUnit;

UCLASS()
class AODD_API UUnitHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	ABaseUnit* PlayerOwner;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* MaxHealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* HealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(EditDefaultsOnly)
		UCurveLinearColor* HealthColorCurve;

	virtual void NativeConstruct() override;

private:
	void ChangeHealth(float Health);
};
