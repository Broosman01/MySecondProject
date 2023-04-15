// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class AODD_API UWinMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	virtual void NativeConstruct() override;
private:

	UFUNCTION()
	void Quit();

	void SaveGold();
};
