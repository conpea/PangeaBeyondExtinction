// Fill out your copyright notice in the Description page of Project Settings.


#include "TestWorldClass.h"



TestWorldClass::TestWorldClass()
{
}



TestWorldClass::~TestWorldClass()
{
}



bool TestWorldClass::testSave(FVector inVec, FRotator inRot)
{
    FString debugStr = FString(TEXT("Saving..."));
    GEngine->AddOnScreenDebugMessage(11, 3.0f, FColor::Cyan, debugStr);
    UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);

    USaveGameFile* SaveGameInstance = Cast<USaveGameFile>(UGameplayStatics::CreateSaveGameObject(USaveGameFile::StaticClass()));
    if (SaveGameInstance)
    {
        // Set up the (optional) delegate.
        FAsyncSaveGameToSlotDelegate SavedDelegate;
        // USomeUObjectClass::SaveGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, bool bSuccess
        SavedDelegate.BindUObject(SaveGameInstance, &USaveGameFile::finishedResponseSave); // use created save's function

        // Set data on the savegame object.
        SaveGameInstance->playerPosition = inVec;
        SaveGameInstance->playerRotation = inRot;

        // Start async save process.
        UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveGameInstance->str_saveSlot, SaveGameInstance->int_userIndex, SavedDelegate);
        //UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveGameInstance->str_saveSlot, SaveGameInstance->int_userIndex); // possible without delegate?

        /*
        // synchronous save
        bool saveSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->str_saveSlot, SaveGameInstance->int_userIndex);
        if (saveSuccess)
        {
        }
        */
    }

    debugStr = FString(TEXT("Save initiation complete"));
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, debugStr);
    UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);

    return true;
}



bool TestWorldClass::testLoad(ACharacter *thisCharacter)
{
    FString debugStr = FString(TEXT("Loading...."));
    UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, debugStr);

    // load game - use player selected slot name and index
    // test values
    FString str_saveSlot = TEXT("DefaultSlot");
    uint32 int_userIndex = 0;

    // async load
    // save references to game parts that will be updated by load later, if not saved already
    TestWorldClass::_userCharacter = thisCharacter;
    // create USaveGameFile object, similar to when saving, for delegate function
    USaveGameFile * testSaveGame = Cast<USaveGameFile>(UGameplayStatics::CreateSaveGameObject(USaveGameFile::StaticClass()));
    FAsyncLoadGameFromSlotDelegate LoadedDelegate;
    // USomeUObjectClass::LoadGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData
    LoadedDelegate.BindUObject(testSaveGame, &USaveGameFile::finishedResponseLoad);
    UGameplayStatics::AsyncLoadGameFromSlot(str_saveSlot, int_userIndex, LoadedDelegate);
    /*

    // synchronous load
    USaveGameFile * LoadedGame = Cast<USaveGameFile>(UGameplayStatics::LoadGameFromSlot(str_saveSlot, int_userIndex));
    if (LoadedGame)
    {
        // use data
        //LoadedGame->PlayerName
        thisCharacter->SetActorLocation(LoadedGame->playerPosition);
        thisCharacter->SetActorRotation(LoadedGame->playerRotation);
    }
    else
    {
    }
    */

    debugStr = FString(TEXT("Load initiation complete"));
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, debugStr);
    UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);

    return true;
}



void TestWorldClass::finishedResponseSave(const FString & inSlotName, const int32 inUserIndex, bool isSuccess)
{
    if (isSuccess)
    {
        FString debugStr = FString(TEXT("Saving successful!"));
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, debugStr);
        UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
    }
    else
    {
        FString debugStr = FString(TEXT("WARNING: Saving failed"));
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, debugStr);
        UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
    }
}



void TestWorldClass::finishedResponseLoad(const FString & inSlotName, const int32 inUserIndex, USaveGame * inSaveGame)
{
    if (inSaveGame)
    {
        // cast SaveGame object to USaveGameFile
        //USaveGameFile * inMySaveGame = (USaveGameFile*)inSaveGame; // classic C++ casting; works too?
        USaveGameFile * inMySaveGame = Cast<USaveGameFile>(inSaveGame); // given method for casting

        FString resultStr = FString(TEXT("File loaded, now applying..."));
        resultStr += FString(TEXT("\n\t")) + inMySaveGame->playerPosition.ToString();
        resultStr += FString(TEXT("\n\t")) + inMySaveGame->playerRotation.ToString();
        UE_LOG(LogClass, Log, TEXT("%s"), *resultStr);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, resultStr);

        // use data
        TestWorldClass::_userCharacter->SetActorLocation(inMySaveGame->playerPosition);
        TestWorldClass::_userCharacter->SetActorRotation(inMySaveGame->playerRotation);
        
        resultStr = FString(TEXT("Load successful!"));
        UE_LOG(LogClass, Log, TEXT("%s"), *resultStr);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, resultStr);
    }
    else
    {
        FString resultStr = FString(TEXT("No matching saved file found successfully :/"));
        UE_LOG(LogClass, Log, TEXT("%s"), *resultStr);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, resultStr);
    }
}
