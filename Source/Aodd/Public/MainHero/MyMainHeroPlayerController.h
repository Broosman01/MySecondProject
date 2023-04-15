// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyMainHeroPlayerController.generated.h"


UCLASS()
class AODD_API AMyMainHeroPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	AAoddGameModeBase* GameMode;
private:
	void OnPauseGame();

	UFUNCTION()
	void OnClearPause();

	UFUNCTION()
	void OnLvlComplete();

	UFUNCTION()
	void OnLvlLose();
};
