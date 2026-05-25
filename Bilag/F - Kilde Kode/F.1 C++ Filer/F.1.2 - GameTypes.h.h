// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	Resistance UMETA(DisplayName = "Resistance"),
	SS UMETA(DisplayName = "SS")
};

UENUM(BlueprintType)
enum class EPlayerClass : uint8
{
    None UMETA(DisplayName = "None"),

    // Resistance classes
    LanguageExpert UMETA(DisplayName = "Language Expert"),
    ScienceExpert UMETA(DisplayName = "Science Expert"),
    SportsExpert UMETA(DisplayName = "Sports Expert"),
    MusicExpert UMETA(DisplayName = "Music Expert"),

    // SS classes
    Officer UMETA(DisplayName = "Officer"),
    Soldier UMETA(DisplayName = "Soldier"),
    Intelligence UMETA(DisplayName = "Intelligence")
};

class SEP4_API GameTypes
{
public:
	GameTypes();
	~GameTypes();
};
