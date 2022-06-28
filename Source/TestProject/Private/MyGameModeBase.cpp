// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPlayerController.h"
#include "MyPawn.h"
#include "MyHUD.h"

AMyGameModeBase::AMyGameModeBase() //: Super()
{
	ConstructorHelpers::FClassFinder<AMyPlayerController> PlayerControllerClassFinder(TEXT("/Script/TestProject.MyPlayerController"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;

	//	ConstructorHelpers::FClassFinder<AMyCharacter> PawnClassFinder(TEXT("/Script/TestProject.MyCharacter"));
	//	DefaultPawnClass = PawnClassFinder.Class;

	ConstructorHelpers::FClassFinder<AMyHUD> HUDClassFinder(TEXT("/Script/TestProject.MyHUD"));
	HUDClass = HUDClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/CharacterCPP/BP_MyCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/TestProject.MyCharacter"));
	DefaultPawnClass = PlayerPawnBPClass.Class;*/
}
