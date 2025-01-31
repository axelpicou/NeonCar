// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeonCarWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UNeonCarWheelRear::UNeonCarWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}