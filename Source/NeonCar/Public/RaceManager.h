// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaceManager.generated.h"

UCLASS()
class NEONCAR_API ARaceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaceManager();
	// Update Classement of race
	UFUNCTION(BlueprintCallable, Category="RaceManager")
	void UpdateRacePositions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:		
	// Array all Vehicles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race")
	TArray<class ANeonCarPawn*> Vehicles;
};
