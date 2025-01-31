// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeonCarWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UNeonCarWheelFront::UNeonCarWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}