// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Race/RaceManager.h"
#include "AIVehicleControlle.generated.h"

/**
 * 
 */
UCLASS()
class NEONCAR_API AAIVehicleControlle : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void FollowSplinePath();

private:
	UPROPERTY(EditAnywhere,Category = "AI")
	AActor* TargetSpline;
	
	ARaceManager* RaceManager;
	
	int32 CurrentTargetIndex;
};
