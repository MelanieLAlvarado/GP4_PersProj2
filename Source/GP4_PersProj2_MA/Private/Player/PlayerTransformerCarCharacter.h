// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChaosWheeledVehicleMovementComponent.h"
#include "CoreMinimal.h"
#include "Player/BPlayerCharacter.h"
#include "PlayerTransformerCarCharacter.generated.h"

/**
 * 
 */
UCLASS()
class APlayerTransformerCarCharacter : public ABPlayerCharacter
{
	GENERATED_BODY()

public:
	APlayerTransformerCarCharacter();
private:
	virtual void HandleTransformInput(const struct FInputActionValue& InputActionValue) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Transform")
	UCharacterMovementComponent* CharacterMovementComp;

	UPROPERTY(EditDefaultsOnly, Category = "Transform")
	UChaosVehicleMovementComponent* VehicleMovementComp;

};
