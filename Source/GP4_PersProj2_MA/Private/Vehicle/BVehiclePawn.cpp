// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/BVehiclePawn.h"
#include "Camera/CameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"

ABVehiclePawn::ABVehiclePawn()
{
	UChaosWheeledVehicleMovementComponent* VehicleMovementComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

	//VehicleMovementComp->

	//adjust tire load
	/*VehicleMovementComp->MinNormalizedTireLoad = 0.0f;
	VehicleMovementComp->MinNormalizedTireLoadFiltered = 0.2f;
	VehicleMovementComp->MaxNormalizedTireLoad = 2.0f;
	VehicleMovementComp->MaxNormalizedTireLoadFiltered = 2.0f;*/

	if (VehicleMovementComp)
	{
		//torque setup
		VehicleMovementComp->EngineSetup.MaxRPM = 5700.f;
		VehicleMovementComp->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
		VehicleMovementComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 4000.f);
		VehicleMovementComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.f);
		VehicleMovementComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.f);

		//adjust steering
		VehicleMovementComp->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
		VehicleMovementComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 1.f);
		VehicleMovementComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(40.0f, .7f);
		VehicleMovementComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

		VehicleMovementComp->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
		VehicleMovementComp->DifferentialSetup.FrontRearSplit = 0.65f;

		//automatic gearbox
		VehicleMovementComp->TransmissionSetup.bUseAutomaticGears = true;
		VehicleMovementComp->TransmissionSetup.GearChangeTime = 0.15f;
		//VehicleMovementComp->TransmissionSetup.GearAutoBoxLatency = 1.0f;
	}

	//Camera boom
	VehicleCamBoom = CreateDefaultSubobject<USpringArmComponent>("Vehicle Camera Boom");
	VehicleCamBoom->SetupAttachment(GetRootComponent());
	VehicleCamBoom->TargetArmLength = 250.f;
	VehicleCamBoom->bUsePawnControlRotation = true;

	//Camera
	VehicleCam = CreateDefaultSubobject<UCameraComponent>("Vehicle Camera");
	VehicleCam->SetupAttachment(VehicleCamBoom);
	VehicleCam->FieldOfView = 90.f;
}

void ABVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateInAirControl(DeltaTime);
}

void ABVehiclePawn::PawnClientRestart()
{
	Super::PawnClientRestart();
	APlayerController* OwningPlayerController = GetController<APlayerController>();
	if (OwningPlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
			OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		EnhancedSubsystem->RemoveMappingContext(GameplayInputMappingContext);
		EnhancedSubsystem->AddMappingContext(GameplayInputMappingContext, 0);
	}
}

void ABVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ABVehiclePawn::HandleMoveInput);

		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ABVehiclePawn::HandleLookInput);
		/*EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::Jump);

		EnhancedInputComponent->BindAction(QuitInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleQuitInput);
		EnhancedInputComponent->BindAction(TransformInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleQuitInput);*/
	}
}

void ABVehiclePawn::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	InputValue.Normalize();

	GetVehicleMovementComponent()->SetSteeringInput(InputValue.X);
	
	//is braking the car
	if (InputValue.Y < 0.0f)
	{
		OnHandBrakePressed();
		return;
	}
	
	OnHandBrakeReleased();
	GetVehicleMovementComponent()->SetThrottleInput(InputValue.Y);
}

void ABVehiclePawn::HandleLookInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-InputValue.Y);
}

void ABVehiclePawn::OnHandBrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ABVehiclePawn::OnHandBrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ABVehiclePawn::UpdateInAirControl(float DeltaTime)
{
	//Add Later...
}

FVector ABVehiclePawn::GetLookRightDirection() const
{
	return VehicleCam->GetRightVector();
}

FVector ABVehiclePawn::GetLookForwardDirection() const
{
	return VehicleCam->GetForwardVector();
}