// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "MyPawn.h"
#include "MyHUD.h"

AMyGameModeBase::AMyGameModeBase() //: Super()
{
	ConstructorHelpers::FClassFinder<AMyPlayerController> PlayerControllerClassFinder(TEXT("/Script/TestProject.MyPlayerController"));

	ConstructorHelpers::FClassFinder<AMyCharacter> PawnClassFinder(TEXT("/Script/TestProject.MyCharacter"));
	ConstructorHelpers::FClassFinder<AMyHUD> HUDClassFinder(TEXT("/Script/TestProject.MyHUD"));
	
	PlayerControllerClass = PlayerControllerClassFinder.Class;
	DefaultPawnClass = PawnClassFinder.Class;
	HUDClass = HUDClassFinder.Class;
}
