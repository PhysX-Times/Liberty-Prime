// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "LibertyPrimeGameMode.h"
#include "LibertyPrimePlayerController.h"
#include "LibertyPrimeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALibertyPrimeGameMode::ALibertyPrimeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ALibertyPrimePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/PlayerCharBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}