// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPangea.h"
#include "SaveGameFile.h"
#include "GameFramework/Character.h"

/**
Test "world" class for both handling saving/loading functions and sourcing relevant game data in one place.
Input to save or load activates functions here.
"delegate" callbacks are also pointed to functions provided here, for handling results of asynchronous saving/loading

TODO: how to access relevant data for saving/loading from here?
 */
class PROJECTPANGEA_API TestWorldClass
{
private:
    // pointers to data/data sources to be saved/loaded
    static ACharacter * _userCharacter;


public:
	TestWorldClass();
	~TestWorldClass();

    static bool testSave(FVector inVec, FRotator inRot); // parameters depend on what is needed to be saved; eventually clearing?
    static bool testLoad(ACharacter *thisCharacter); // parameters depend on what is needed to be loaded; eventually clearing?

    static void finishedResponseSave(const FString & inSlotName, const int32 inUserIndex, bool isSuccess);
    static void finishedResponseLoad(const FString & inSlotName, const int32 inUserIndex, USaveGame * inSaveGame);
};

ACharacter * TestWorldClass::_userCharacter = 0;
