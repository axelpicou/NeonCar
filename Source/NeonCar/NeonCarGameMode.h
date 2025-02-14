// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "Race/RaceManager.h"
#include "NeonCarGameMode.generated.h"

UCLASS(MinimalAPI)
class ANeonCarGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANeonCarGameMode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Race")
	ARaceManager* RaceManager;
};



