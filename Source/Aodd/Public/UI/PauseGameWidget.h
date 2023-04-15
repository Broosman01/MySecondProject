// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseGameWidget.generated.h"

class UButton;

UCLASS()
class AODD_API UPauseGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:


protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void ClearPause();

	UFUNCTION()
	void QuitLvl();
};
