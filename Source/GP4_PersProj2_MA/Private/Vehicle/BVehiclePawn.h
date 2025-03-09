// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "BVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class ABVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	ABVehiclePawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Throttle/Steering
	void ApplyThrottle(float Val);
	void ApplySteering(float Val);

	//Controls
	void LookUp(float Val);
	void Turn(float Val);

	//HandBrake
	void OnHandBrakePressed();
	void OnHandBrakeReleased();

	//Air Physics
	void UpdateInAirControl(float DeltaTime);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle Camera")
	class USpringArmComponent* VehicleCamBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Vehicle Camera")
	class UCameraComponent* VehicleCam;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ThrottleInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SteerInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* HandBrakePressInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* HandBrakeReleaseInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* GameplayInputMappingContext;
};
