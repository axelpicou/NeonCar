// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeonCarGameMode.h"
#include "NeonCarPlayerController.h"

ANeonCarGameMode::ANeonCarGameMode()
{
	PlayerControllerClass = ANeonCarPlayerController::StaticClass();
}
