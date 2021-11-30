// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameFile.h"



USaveGameFile::USaveGameFile()
{
    str_saveSlot = TEXT("DefaultSlot");
    int_userIndex = 0;
}



/*
Delegate called from AsyncLoadGameFromSlot.
First two parameters are passed in SlotName and UserIndex, third parameter is a bool indicating success (true) or failure (false).

*/
void USaveGameFile::finishedResponseSave(const FString & inSlotName, const int32 inUserIndex, bool isSuccess)
{
    TestWorldClass::finishedResponseSave(inSlotName, inUserIndex, isSuccess);
}



void USaveGameFile::finishedResponseLoad(const FString & inSlotName, const int32 inUserIndex, USaveGame * inLoadedGame)
{
    TestWorldClass::finishedResponseLoad(inSlotName, inUserIndex, inLoadedGame);
}

