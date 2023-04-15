// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AoddHUD.generated.h"

UCLASS()
class AODD_API AAoddHUD : public AHUD
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayeHudWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WinMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LoseMenuWidgetClass;

	UUserWidget* PlayerHUDWidget;
	UUserWidget* PauseWidget;
	UUserWidget* WinMenuWidget;
	UUserWidget* LoseMenuWidget;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void SetPauseWidget();

	UFUNCTION()
	void ResetPauseWidget();

	UFUNCTION()
	void SetWinMenuWidget();

	UFUNCTION()
	void SetLoseMenuWidget();
};
