// Fill out your copyright notice in the Description page of Project Settings.


#include "Race/RaceManager.h"

#include "HeadMountedDisplayTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

void ARaceManager::OnFinishRace(ANeonCarPawn* FinishedVehicle)
{
	int32 Position = 0;
	if (!FinishedVehicles.Contains(FinishedVehicle))
	{
		FinishedVehicles.Add(FinishedVehicle);
		if (FinishedVehicle->GetName() == "Player")
		{
			Position = FinishedVehicles.Num();
		}
		UE_LOG(LogTemp, Warning, TEXT("%s a terminé la course en position %d"), *FinishedVehicle->GetName(), FinishedVehicles.Num());
	}

	// Vérifie si tous les véhicules ont terminé
	if (FinishedVehicles.Num() == Vehicles.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tous les véhicules ont terminé !"));

		// Déclare le vainqueur (le premier du tableau)
		ANeonCarPawn* Winner = FinishedVehicles[0];		
	}
}

bool ARaceManager::ValideChecpoint(ANeonCarPawn* Vehicle, int32 CheckpointIndex)
{
	if (LastCheckpoint.Contains(Vehicle))
	{
		int32 LastIndex = LastCheckpoint[Vehicle];
		FVector ResetLocation = Vehicle->GetActorLocation();
		FRotator ResetRotation = Vehicle->GetActorRotation();

		Vehicle->SetLocation(ResetLocation);
		Vehicle->SetRotation(ResetRotation);
		
		// Vérifie que le checkpoint est bien le suivant
		if (CheckpointIndex == (LastIndex + 1) % TotalCheckpoints)
		{
			
			LastCheckpoint[Vehicle] = CheckpointIndex;
			UE_LOG(LogTemp, Warning, TEXT("%s a validé le checkpoint %d"), *Vehicle->GetName(), CheckpointIndex);

			// Si c'est le dernier checkpoint, le tour est terminé
			if (CheckpointIndex == TotalCheckpoints - 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s a complété un tour !"), *Vehicle->GetName());
				LastCheckpoint.Reset();
				return false;
			}
			return false;
		}
		else
		{
			Vehicle->ResetVehicle();
			return true;
		}
	}
	else
	{
		// Premier checkpoint pour ce véhicule
		LastCheckpoint.Add(Vehicle, CheckpointIndex);
		return false;
	}
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

