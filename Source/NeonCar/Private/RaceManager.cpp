// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceManager.h"

#include "Kismet/GameplayStatics.h"
#include "NeonCar/NeonCarPawn.h"
#include "NeonCar/NeonCarSportsCar.h"

// Sets default values
ARaceManager::ARaceManager() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARaceManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundVehicles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANeonCarPawn::StaticClass(), FoundVehicles);

	for (AActor* Actor : FoundVehicles)
	{
		if (ANeonCarPawn* Vehicle = Cast<ANeonCarPawn>(Actor))
		{
			Vehicles.Add(Vehicle);
		}
	}
}

// Called every frame
void ARaceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARaceManager::UpdateRacePositions()
{
	for ( ANeonCarPawn* Vehicle : Vehicles)
	{
		Vehicle->UpdateRaceProgress();
	}
	
	// Trier les véhicules par distance parcourue sur la spline (ordre décroissant)
	Vehicles.Sort([](const ANeonCarPawn& A, const ANeonCarPawn& B)
	{
		return A.GetDistance() > B.GetDistance();
	});
}

