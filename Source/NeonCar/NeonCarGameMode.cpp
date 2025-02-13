// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeonCarGameMode.h"
#include "NeonCarPlayerController.h"
#include "RaceManager.h"
#include "Kismet/GameplayStatics.h"

ANeonCarGameMode::ANeonCarGameMode()
{
	PlayerControllerClass = ANeonCarPlayerController::StaticClass();
}

void ANeonCarGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARaceManager::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		RaceManager = Cast<ARaceManager>(FoundActors[0]);
		UE_LOG(LogTemp, Warning, TEXT("RaceManager trouvé et assigné."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RaceManager introuvable dans la scène !"));
	}
}
