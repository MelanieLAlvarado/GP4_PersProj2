// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerTransformerCarCharacter.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerTransformerCarCharacter::APlayerTransformerCarCharacter()
{
	CharacterMovementComp = GetCharacterMovement();
	VehicleMovementComp = CreateDefaultSubobject<UChaosVehicleMovementComponent>("Car Movement Component");

}

void APlayerTransformerCarCharacter::HandleTransformInput(const FInputActionValue& InputActionValue)
{
	if (bIsTransformed)
	{
		VehicleMovementComp->Deactivate();
		CharacterMovementComp->Activate();
	}
	else
	{
		CharacterMovementComp->Deactivate();
		VehicleMovementComp->Activate();
	}
}
