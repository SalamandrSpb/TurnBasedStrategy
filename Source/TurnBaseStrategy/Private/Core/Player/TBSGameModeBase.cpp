// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/TBSGameModeBase.h"
#include "Core/Player/TBSCameraPawn.h"
#include "Core/Player/TBSPlayerController.h"



ATBSGameModeBase::ATBSGameModeBase()
{
	DefaultPawnClass = ATBSCameraPawn::StaticClass();
	PlayerControllerClass = ATBSPlayerController::StaticClass();
}
