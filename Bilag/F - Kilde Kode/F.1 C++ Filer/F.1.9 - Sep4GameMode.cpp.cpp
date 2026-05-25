// Copyright Epic Games, Inc. All Rights Reserved.

#include "Sep4GameMode.h"
#include "Sep4Character.h"
#include "UObject/ConstructorHelpers.h"

ASep4GameMode::ASep4GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
