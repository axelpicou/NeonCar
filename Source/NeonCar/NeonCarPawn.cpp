// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeonCarPawn.h"
#include "NeonCarWheelFront.h"
#include "NeonCarWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EngineUtils.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "VehiclePawn"

DEFINE_LOG_CATEGORY(LogTemplateVehicle);

ANeonCarPawn::ANeonCarPawn()
{
	// construct the front camera boom
	FrontSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Front Spring Arm"));
	FrontSpringArm->SetupAttachment(GetMesh());
	FrontSpringArm->TargetArmLength = 0.0f;
	FrontSpringArm->bDoCollisionTest = false;
	FrontSpringArm->bEnableCameraRotationLag = true;
	FrontSpringArm->CameraRotationLagSpeed = 15.0f;
	FrontSpringArm->SetRelativeLocation(FVector(30.0f, 0.0f, 120.0f));

	FrontCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Front Camera"));
	FrontCamera->SetupAttachment(FrontSpringArm);
	FrontCamera->bAutoActivate = false;

	// construct the back camera boom
	BackSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Back Spring Arm"));
	BackSpringArm->SetupAttachment(GetMesh());
	BackSpringArm->TargetArmLength = 650.0f;
	BackSpringArm->SocketOffset.Z = 150.0f;
	BackSpringArm->bDoCollisionTest = false;
	BackSpringArm->bInheritPitch = false;
	BackSpringArm->bInheritRoll = false;
	BackSpringArm->bEnableCameraRotationLag = true;
	BackSpringArm->CameraRotationLagSpeed = 2.0f;
	BackSpringArm->CameraLagMaxDistance = 50.0f;

	BackCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Back Camera"));
	BackCamera->SetupAttachment(BackSpringArm);

	// Configure the car mesh
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	// get the Chaos Wheeled movement component
	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

}

void ANeonCarPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// steering
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &ANeonCarPawn::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &ANeonCarPawn::Steering);


		// throttle
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ANeonCarPawn::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ANeonCarPawn::Throttle);

		// break 
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ANeonCarPawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &ANeonCarPawn::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ANeonCarPawn::StopBrake);

		// handbrake 
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &ANeonCarPawn::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ANeonCarPawn::StopHandbrake);

		// look around 
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ANeonCarPawn::LookAround);

		// toggle camera 
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &ANeonCarPawn::ToggleCamera);

		// reset the vehicle 
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &ANeonCarPawn::ResetVehicleInput);
	}
	else
	{
		UE_LOG(LogTemplateVehicle, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANeonCarPawn::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARaceManager> It(GetWorld()); It;)
	{
		RaceManager = *It;
		break;
	}
}

void ANeonCarPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag("CircuitSpline")) // Ajoute ce tag à ta spline dans l’éditeur
		{
			TargetSpline = *It;
			break;
		}
	}
	
	// add some angular damping if the vehicle is in midair
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);

	// realign the camera yaw to face front
	float CameraYaw = BackSpringArm->GetRelativeRotation().Yaw;
	CameraYaw = FMath::FInterpTo(CameraYaw, 0.0f, Delta, 1.0f);

	BackSpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 0.0f));

	UpdateRaceProgress();
}

void ANeonCarPawn::Steering(const FInputActionValue& Value)
{
	if (RaceManager && RaceManager->bRaceStarted)
	{
		// get the input magnitude for steering
		float SteeringValue = Value.Get<float>();
	
		// add the input
		ChaosVehicleMovement->SetSteeringInput(SteeringValue);
	}
	
}

void ANeonCarPawn::Throttle(const FInputActionValue& Value)
{
	if (RaceManager && RaceManager->bRaceStarted)
	{
		// get the input magnitude for the throttle
		float ThrottleValue = Value.Get<float>();
	
		// add the input
		ChaosVehicleMovement->SetThrottleInput(ThrottleValue);
	}
	
}

void ANeonCarPawn::Brake(const FInputActionValue& Value)
{
	if (RaceManager && RaceManager->bRaceStarted)
	{
		// get the input magnitude for the brakes
		float BreakValue = Value.Get<float>();
	
		// add the input
		ChaosVehicleMovement->SetBrakeInput(BreakValue);
	}
	
}

void ANeonCarPawn::StartBrake(const FInputActionValue& Value)
{
	if (RaceManager && RaceManager->bRaceStarted)
	{
		// call the Blueprint hook for the break lights
		BrakeLights(true);
	}
	
}

void ANeonCarPawn::StopBrake(const FInputActionValue& Value)
{
	if (RaceManager && RaceManager->bRaceStarted)
	{
		// call the Blueprint hook for the break lights
		BrakeLights(false);

		// reset brake input to zero
		ChaosVehicleMovement->SetBrakeInput(0.0f);
	}
}

void ANeonCarPawn::StartHandbrake(const FInputActionValue& Value)
{
	if (RaceManager && RaceManager->bRaceStarted)
	{
		// add the input
		ChaosVehicleMovement->SetHandbrakeInput(true);

		// call the Blueprint hook for the break lights
		BrakeLights(true);
	}
	
}

void ANeonCarPawn::StopHandbrake(const FInputActionValue& Value)
{
	if (RaceManager && RaceManager->bRaceStarted)
	{
		// add the input
		ChaosVehicleMovement->SetHandbrakeInput(false);

		// call the Blueprint hook for the break lights
		BrakeLights(false);
	}
	
}

void ANeonCarPawn::LookAround(const FInputActionValue& Value)
{
	// get the flat angle value for the input 
	float LookValue = Value.Get<float>();

	// add the input
	BackSpringArm->AddLocalRotation(FRotator(0.0f, LookValue, 0.0f));
}

void ANeonCarPawn::ToggleCamera(const FInputActionValue& Value)
{
	// toggle the active camera flag
	bFrontCameraActive = !bFrontCameraActive;

	FrontCamera->SetActive(bFrontCameraActive);
	BackCamera->SetActive(!bFrontCameraActive);
}

void ANeonCarPawn::ResetVehicleInput(const FInputActionValue& Value)
{
	ResetVehicle();
}

void ANeonCarPawn::ResetVehicle()
{
	// reset to a location slightly above our current one
	FVector ResetLocation = LocationReset;

	// reset to our yaw. Ignore pitch and roll
	FRotator ResetRotation = RotationReset;
	
	// teleport the actor to the reset spot and reset physics
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);

	UE_LOG(LogTemplateVehicle, Error, TEXT("Reset Vehicle"));
}

void ANeonCarPawn::UpdateRaceProgress()
{
	if (!TargetSpline) return;

	USplineComponent* Spline = TargetSpline->FindComponentByClass<USplineComponent>();
	if (!Spline) return;

	DistanceOnSpline = Spline->FindInputKeyClosestToWorldLocation(GetActorLocation());
}

void ANeonCarPawn::OnCrossFinishLine()
{
	if (RaceManager)
	{
		RaceManager->OnFinishRace(this);
	}
}

FVector ANeonCarPawn::SetLocation(FVector newLocation)
{
	LocationReset = newLocation;
	return LocationReset;
}

FRotator ANeonCarPawn::SetRotation(FRotator newRotation)
{
	RotationReset = newRotation;
	return RotationReset;
}

#undef LOCTEXT_NAMESPACE
