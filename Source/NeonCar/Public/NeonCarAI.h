// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NeonCar/NeonCarPawn.h"
#include "NeonCarAI.generated.h"

/**
 * 
 */
UCLASS()
class NEONCAR_API ANeonCarAI : public ANeonCarPawn
{
	GENERATED_BODY()

	/** Cast pointer to the Chaos Vehicle movement component */
	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;

protected:

private:

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
