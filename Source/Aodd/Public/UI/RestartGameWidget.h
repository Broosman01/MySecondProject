// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartGameWidget.generated.h"

class UButton;

UCLASS()
class AODD_API URestartGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:
	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void Back();
};
