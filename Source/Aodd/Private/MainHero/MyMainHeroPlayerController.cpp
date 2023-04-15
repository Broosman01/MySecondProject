// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHero/MyMainHeroPlayerController.h"
#include "Gameframework/GameModeBase.h"
#include "Aodd/AoddGameModeBase.h"

void AMyMainHeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
}

void AMyMainHeroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &AMyMainHeroPlayerController::OnPauseGame);

    GameMode = Cast<AAoddGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnPauseReset.AddUObject(this, &AMyMainHeroPlayerController::OnClearPause);
		GameMode->OnWinMenuSet.AddUObject(this, &AMyMainHeroPlayerController::OnLvlComplete);
		GameMode->OnLoseMenuSet.AddUObject(this, &AMyMainHeroPlayerController::OnLvlComplete);
	}
}

void AMyMainHeroPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode() || GameMode->LvlComplete) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void AMyMainHeroPlayerController::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode() || GameMode->LvlComplete) return;

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void AMyMainHeroPlayerController::OnLvlComplete()
{
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void AMyMainHeroPlayerController::OnLvlLose()
{
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}