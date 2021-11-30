// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeOfDaySystem.h"
#include "Math/UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Math/Rotator.h"

// Sets default values
ATimeOfDaySystem::ATimeOfDaySystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorHiddenInGame(true);

	UStaticMeshComponent* RootComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	RootComponent = RootComp;

	SunPathComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SunPathComp"));
	SunPathComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SunDialComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SunDialComp"));
	SunDialComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SunDialComp->SetupAttachment(SunPathComp);
		
}

void ATimeOfDaySystem::OnConstruction(const FTransform& Transform)
{
	
	Super::OnConstruction(Transform);

	// We only want to override the current time if we're in the editor. Otherwise, every time we change a parameter when testing the game, it resets the time
	if (!(GetWorld()->GetGameInstance())) // If we cannot find a Game Instance it means we are are in editor and not playing/simulating
	{

		CurrentTimeSeconds = StartingTime;
		
	}
	
	UpdateTime();
	CalculateSunPosition();
	UpdateSun();	
		
}

// Called when the game starts or when spawned
void ATimeOfDaySystem::BeginPlay()
{
	Super::BeginPlay();

	//We probably want to read the time from the server or a save game at this point

	//CurrentTimeSeconds = StartingTime;
	
}

// Called every frame
void ATimeOfDaySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SunReference == nullptr) // We are loud about it if no sun was found
	{

		UE_LOG(LogTemp, Error, TEXT("No sun was assigned to Time System"));

	}

	CurrentTimeSeconds = CurrentTimeSeconds + (DaySpeed * DeltaTime); // Every frame, we add the time that has passed since the previous one, corrected using our Day Speed
	
	UpdateTime();
	CalculateSunPosition();
	UpdateSun();

}

void ATimeOfDaySystem::UpdateTime()
{
	
	if (CurrentTimeSeconds >= (24 * 3600)) // If it's after midnight...
	{
		CurrentTimeSeconds -= (24 * 3600); // ...subtract the duration of a day

		CurrentDayOfYear++; // ... and add a day

		if (CurrentDayOfYear > 365) // If it's after New Year...
		{
			CurrentDayOfYear -= 365; // ... subtract the duration of a year
		}
	}

	// Express the current time as HH:MM so it's easy to read by the designer and ready to be picked up by the UI 
	
	CurrentTimeHours = FMath::Floor(CurrentTimeSeconds / 3600);	
	
	CurrentTimeMinutes = FMath::Floor((CurrentTimeSeconds - (CurrentTimeHours * 3600)) / 60);

	CurrentTimeNormalized = CurrentTimeSeconds / (24 * 3600); // Time of the day on a 0-1 range

}

void ATimeOfDaySystem::CalculateSunPosition()
{
	// First, we get angle of the plane containing the path of the sun during a given day, interpolating between solstices

	//// We offset the Day of the year so the first day corresponds to 21st of December (Winter Solstice)
	int OffsetDayOfYear = CurrentDayOfYear + 11;

	// And make sure that those days at the end are still in the 0-364 range
	if (OffsetDayOfYear > 365) OffsetDayOfYear -= 365; 
	if (OffsetDayOfYear <= 0) OffsetDayOfYear += 365;

	if (OffsetDayOfYear <= 182) // Day before 21st of June, that day is after the winter solstice and before the summer solstice
	{

		CurrentSunPathAngle = FMath::Lerp(WinterSunPathAngle, SummerSunPathAngle, float(OffsetDayOfYear) / 182.0); //182 is half of the 365 days in a year
	
	}

	else // Otherwise, that day is after the summer solstice and before the winter solstice
	{

		CurrentSunPathAngle = FMath::Lerp(SummerSunPathAngle, WinterSunPathAngle, (float(OffsetDayOfYear) - 182.0) / 182.0);

	}

}

void ATimeOfDaySystem::UpdateSun()
{

	//We update the visual for the sun path (relative, so we can position the north accordingly)
	SunPathComp->SetRelativeRotation(FRotator(0.0, 0.0, CurrentSunPathAngle));
	
	FRotator DialRotator;
	DialRotator.Roll = 0.0;
	DialRotator.Pitch = 0.0;
	DialRotator.Yaw = FMath::Lerp(-90.0, 270.0, CurrentTimeNormalized); //-90 for midnight, 0 for 6am, 90 for noon, 180 for 9pm, 270 for midnight again
	
	//We update the dial on the sun path. Since it is a child of it, it will be contained in the same plane
	SunDialComp->SetRelativeRotation(DialRotator);

	//And since the Dial is now rotated from the parent to account for the day of the year and itself for the time of the day, we can use its absolute rotation to orient the sun
	if (SunReference != nullptr)
	{
		SunRotation = SunDialComp->GetComponentRotation();
		
		SunReference->SetActorRotation(SunRotation);

	}
}
