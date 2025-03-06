// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BPlayerController.h"
#include "CoreMinimal.h"
#include "Character/BCharacter.h"
//#include "Widget/GameplayWidget.h"
#include "BPlayerCharacter.generated.h"

//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponUpdated, int /*maxCount*/, int);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponUpdatedDynamic, int, MaxCount, int, Value);

/**
 *
 */
UCLASS()
class ABPlayerCharacter : public ABCharacter
{
	GENERATED_BODY()
public:
	ABPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	//FOnWeaponUpdated OnWeaponUpdated;
	//void WeponUpdated(int a, int b);

	//UFUNCTION()
	//void WeaponUpdatedDynamic(int a, int b);

	//FOnWeaponUpdatedDynamic OnWeaponUpdatedDynamic;

	//UFUNCTION()
	//void InitializeWeaponComponentUI(UGameplayWidget* GameplayWidget);

	UFUNCTION()
	UCameraComponent* GetViewCamera();

	UFUNCTION()
	USpringArmComponent* GetCameraBoom();

private:
	/********************
	*		Camera		*
	*********************/

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class UCameraComponent* ViewCam;

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CamBoomYOffset = 0.0f;

	/********************
	*		Input		*
	*********************/

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* QuitInputAction;


	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* GameplayInputMappingContext;

	void HandleLookInput(const struct FInputActionValue& InputActionValue);
	void HandleMoveInput(const struct FInputActionValue& InputActionValue);

	void HandleQuitInput(const struct FInputActionValue& InputActionValue);


	FVector GetLookRightDirection() const;

	FVector GetLookForwardDirection() const;

	FVector GetMoveForwardDirection() const;
};
