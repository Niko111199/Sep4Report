// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameTypes.h"
#include "MyPlayerState.generated.h"


UCLASS()
class SEP4_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	ETeam team;

	UPROPERTY(BlueprintReadWrite)
	EPlayerClass playerClass;

	void SetTeam(ETeam newTeam);

	void SetPlayerClass(EPlayerClass newClass);
};
