// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NeonCarPawn.h"
#include "NeonCarSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class NEONCAR_API ANeonCarSportsCar : public ANeonCarPawn
{
	GENERATED_BODY()
	
public:

	ANeonCarSportsCar();
};
