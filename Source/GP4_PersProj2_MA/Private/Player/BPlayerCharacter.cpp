// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BPlayerCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h" 
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#define ECC_Target ECC_GameTraceChannel1

ABPlayerCharacter::ABPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SetRelativeLocation(FVector(0, CamBoomYOffset, 0));

	bUseControllerRotationYaw = false;

	ViewCam = CreateDefaultSubobject<UCameraComponent>("View Cam");
	ViewCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(1200.f);
}
void ABPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABPlayerCharacter::PawnClientRestart()
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

// Called to bind functionality to input
void ABPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleLookInput);
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleMoveInput);

		EnhancedInputComponent->BindAction(QuitInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleQuitInput);
		EnhancedInputComponent->BindAction(TransformInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleQuitInput);
	}
}

void ABPlayerCharacter::Tick(float DeltaTime)
{

}

/*void ABPlayerCharacter::InitializeWeaponComponentUI(UGameplayWidget* GameplayWidget)
{
	if (!WeaponComponent)
		return;

	UWeaponInfoWidget* WeaponWidget = GameplayWidget->GetWeaponInfoWidget();
	if (!WeaponWidget)
		return;

	WeaponComponent->SetWeaponInfoWidget(WeaponWidget);
}*/

UCameraComponent* ABPlayerCharacter::GetViewCamera()
{
	return ViewCam;
}

USpringArmComponent* ABPlayerCharacter::GetCameraBoom()
{
	return CameraBoom;
}

void ABPlayerCharacter::HandleLookInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-InputValue.Y);
}

void ABPlayerCharacter::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	if (InputValue.Length() < 0)
	{
		return;
	}
	InputValue.Normalize();

	AddMovementInput(GetLookRightDirection() * InputValue.X + GetMoveForwardDirection() * InputValue.Y);
}

void ABPlayerCharacter::HandleQuitInput(const FInputActionValue& InputActionValue)
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->ConsoleCommand("quit");
	}
}

FVector ABPlayerCharacter::GetLookRightDirection() const
{
	return ViewCam->GetRightVector();
}

FVector ABPlayerCharacter::GetLookForwardDirection() const
{
	return ViewCam->GetForwardVector();
}

FVector ABPlayerCharacter::GetMoveForwardDirection() const
{
	return FVector::CrossProduct(GetLookRightDirection(), FVector::UpVector);
}