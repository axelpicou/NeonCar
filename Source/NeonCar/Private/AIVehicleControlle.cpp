// Fill out your copyright notice in the Description page of Project Settings.


#include "AIVehicleControlle.h"

#include "ChaosVehicleMovementComponent.h"
#include "EngineUtils.h"
#include "RaceManager.h"
#include "WheeledVehiclePawn.h"
#include "Components/SplineComponent.h"

void AAIVehicleControlle::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag("CircuitSpline")) // Ajoute ce tag à ta spline dans l’éditeur
		{
			TargetSpline = *It;
			break;
		}
	}

	for (TActorIterator<ARaceManager> It(GetWorld()); It;)
	{
		RaceManager = *It;
		break;
	}

	CurrentTargetIndex = 0;
}

void AAIVehicleControlle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FollowSplinePath();
}

void AAIVehicleControlle::FollowSplinePath()
{
	if (RaceManager && !RaceManager->bRaceStarted)
	{
	
	}
	else
	{
			AWheeledVehiclePawn* VehiclePawn = Cast<AWheeledVehiclePawn>(GetPawn());
	if (!VehiclePawn || !TargetSpline) return;

	USplineComponent* Spline = TargetSpline->FindComponentByClass<USplineComponent>();
	if (!Spline) return;

	// Obtenir la position et la tangente du point suivant
	FVector TargetLocation = Spline->GetLocationAtSplinePoint(CurrentTargetIndex, ESplineCoordinateSpace::World);
	FVector SplineDirection = Spline->GetTangentAtSplinePoint(CurrentTargetIndex, ESplineCoordinateSpace::World).GetSafeNormal();

	// Position et direction actuelle du véhicule
	FVector VehicleLocation = VehiclePawn->GetActorLocation();
	FVector Forward = VehiclePawn->GetActorForwardVector();
	FVector ToTarget = (TargetLocation - VehicleLocation).GetSafeNormal();

	// Calculer l'angle pour ajuster la direction
	float SteeringValue = FVector::CrossProduct(Forward, ToTarget).Z;

	// ✅ Ajouter le freinage en fonction de la courbure du circuit
	float CurveFactor = FVector::DotProduct(SplineDirection, Forward);
	CurveFactor = FMath::Clamp(CurveFactor, -1.0f, 1.0f); 

	// Déterminer la vitesse cible en fonction de la courbure
	float TargetSpeed = FMath::Lerp(300.0f, 2500.0f, (CurveFactor + 1.0f) / 2.0f); // 500 dans virages serrés, 2500 en ligne droite
	float CurrentSpeed = VehiclePawn->GetVehicleMovementComponent()->GetForwardSpeed();

	// Calculer l'intensité du freinage
	float BrakeValue = 0.0f;
	if (CurrentSpeed > TargetSpeed)
	{
		BrakeValue = FMath::Clamp((CurrentSpeed - TargetSpeed) / 2000.0f, 0.0f, 1.0f);
	}

	// Appliquer le frein et l’accélération
	VehiclePawn->GetVehicleMovementComponent()->SetBrakeInput(BrakeValue);
	VehiclePawn->GetVehicleMovementComponent()->SetThrottleInput(FMath::Clamp(1.0f - BrakeValue, 0.8f, 1.0f)); 
	VehiclePawn->GetVehicleMovementComponent()->SetSteeringInput(SteeringValue * 2.0f); 

	// Vérifier si on est proche du point pour passer au suivant
	if (FVector::Dist(VehicleLocation, TargetLocation) < 1000.f)
	{
		CurrentTargetIndex++;
		if (CurrentTargetIndex >= Spline->GetNumberOfSplinePoints())
		{
			CurrentTargetIndex = 0; // Recommencer la boucle
		}
	}
	}
}
