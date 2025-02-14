// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaceClassementWidget.generated.h"

/**
 * 
 */
UCLASS()
class NEONCAR_API URaceClassementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RaceUI")
	void UpdateLeaderboard(const TArray<class ANeonCarPawn*>& Vehicles);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LeaderboardText;
};
