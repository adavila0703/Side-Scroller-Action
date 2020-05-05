// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "sidescrolling_actionGameMode.h"
#include "sidescrolling_actionCharacter.h"
#include "UObject/ConstructorHelpers.h"

Asidescrolling_actionGameMode::Asidescrolling_actionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
