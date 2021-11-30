// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProjectPangea.h"
#include "TestWorldClass.h"
#include "SaveGameFile.generated.h"

/**
 Game save file.
 Member variables represent data to be saved and loaded.
 Holds functions called by delegates in asynchronous saving/loading.
 Those functions then use functions in the WorldClass to finish delegate follow-up.
 */
UCLASS()
class PROJECTPANGEA_API USaveGameFile : public USaveGame
{
	GENERATED_BODY()
	
public:
    // Default slot name and index
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString str_saveSlot;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 int_userIndex;
	
	// Properties to save
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector playerPosition;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FRotator playerRotation;


    USaveGameFile();
    void finishedResponseSave(const FString &, const int32, bool);
    void finishedResponseLoad(const FString &, const int32, USaveGame *);
};
