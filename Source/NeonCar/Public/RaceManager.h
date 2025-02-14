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

	void DisplayVictoryScreen(int32 Position) const;

	UFUNCTION(BlueprintCallable, Category="FinishRace")
	void OnFinishRace(ANeonCarPawn* FinishedVehicle);

	UFUNCTION(BlueprintCallable)
	bool ValideChecpoint(ANeonCarPawn* Vehicle, int32 CheckpointIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:		
	// Array all Vehicles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race")
	TArray<class ANeonCarPawn*> Vehicles;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FinishRace")
	TArray<ANeonCarPawn*> FinishedVehicles;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	TMap<ANeonCarPawn*, int32> LastCheckpoint;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	int32 TotalCheckpoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRaceStarted = false;
};
