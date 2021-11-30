// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeOfDaySystem.generated.h"

class ADirectionalLight;
class UStaticMeshComponent;
class UStaticMesh;

UCLASS()
class PROJECTPANGEA_API ATimeOfDaySystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeOfDaySystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	//COMPONENTS

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SunPathComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SunDialComp;

	// SETUP

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Setup")
	ADirectionalLight* SunReference;

	UPROPERTY(EditDefaultsOnly, Category = "1. Setup")
	UStaticMesh* SunPathMesh;

	UPROPERTY(EditDefaultsOnly, Category = "1. Setup")
	UStaticMesh* SunDialMesh;

	// TIME OF DAY
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "2. Time of day")
	FRotator SunRotation;

	/** How much faster in-game time flows (x60 means that every real second equals one minute in game) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Day Speed", Category = "2. Time of day")
	float DaySpeed = 60.0f;

	/** Time of the day (in seconds) for the game to start at */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Time of day", meta = (UIMin = "0", UIMax = "86399"))
	float StartingTime = 25200.0; //7 am

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "2. Time of day")
	float CurrentTimeSeconds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "2. Time of day")
	int CurrentTimeHours;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "2. Time of day")
	int CurrentTimeMinutes;

	/** Time of the day on a 0-1 range*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "2. Time of day")
	float CurrentTimeNormalized;
	
	/** Date expressed as days since the beginning of the year (41 means 10th of Feb)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Time of day", meta = (UIMin = "1", UIMax = "365"))
	int CurrentDayOfYear = 21;

	/** Maximum inclination of the sun during the summer solstice (21st June at noon) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Time of day")
	float SummerSunPathAngle = 65.0f;

	/** Maximum inclination of the sun during the winter solstice (21st December at noon) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Time of day")
	float WinterSunPathAngle = 45.0f;

	/** Maximum inclination of the sun on the current date (interpolated from the other values) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "2. Time of day")
	float CurrentSunPathAngle = 45.0f;
	

	// FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "Time of day")
	void UpdateTime();

	UFUNCTION(BlueprintCallable, Category = "Time of day")
	void CalculateSunPosition();

	UFUNCTION(BlueprintCallable, Category = "Time of day")
	void UpdateSun();


};
