// Fill out your copyright notice in the Description page of Project Settings.


#include "Race/RaceClassementWidget.h"

#include "Components/TextBlock.h"
#include "NeonCar/NeonCarPawn.h"

void URaceClassementWidget::UpdateLeaderboard(const TArray<class ANeonCarPawn*>& Vehicles)
{
	FString LeaderboardString = "Classement:\n";

	for (int i = 0; i < Vehicles.Num(); i++)
	{
		LeaderboardString += FString::Printf(TEXT("%d. %s\n"), i + 1, *Vehicles[i]->GetName());
	}

	if (LeaderboardText)
	{
		LeaderboardText->SetText(FText::FromString(LeaderboardString));
	}
}
