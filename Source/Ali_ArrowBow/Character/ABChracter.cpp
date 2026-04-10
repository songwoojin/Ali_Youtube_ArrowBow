// Copyright Epic Games, Inc. All Rights Reserved.

#include "ABChracter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Ali_ArrowBow.h"
#include "Component/ABBowMechanicsComponent.h"

AABChracter::AABChracter()
	:BowMechanicsComponent(nullptr)
	,bIsBowAiming(false)
	,DefaultFOV(90.0f)
	,AimFOV(60.0f)
	,AimBoomOffset(FVector(100.0f,75.0f,45.0f))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	BowMechanicsComponent=CreateDefaultSubobject<UABBowMechanicsComponent>(TEXT("BowMechanicsComponent"));

	InitialBoomOffset=CameraBoom->SocketOffset;
}

void AABChracter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABChracter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AABChracter::Look);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABChracter::Look);
		
		EnhancedInputComponent->BindAction(BowAimAction, ETriggerEvent::Started, this, &AABChracter::BowAimBegin);
		EnhancedInputComponent->BindAction(BowAimAction, ETriggerEvent::Completed, this, &AABChracter::BowAimEnd);
	}
	else
	{
		UE_LOG(LogAli_ArrowBow, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AABChracter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraZoomInterp(DeltaTime);
}

void AABChracter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	DoMove(MovementVector.X, MovementVector.Y);
}

void AABChracter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AABChracter::BowAimBegin(const FInputActionValue& Value)
{
	BowMechanicsComponent->AimBegin();
	bIsBowAiming=true;
	//AimCameraBegin();
}

void AABChracter::BowAimEnd(const FInputActionValue& Value)
{
	BowMechanicsComponent->AimEnd();
	bIsBowAiming=false;
	//AimCameraEnd();
}

void AABChracter::AimCameraBegin()
{
	FollowCamera->SetFieldOfView(AimFOV);
	CameraBoom->SocketOffset=AimBoomOffset;
}

void AABChracter::AimCameraEnd()
{
	FollowCamera->SetFieldOfView(DefaultFOV);
	CameraBoom->SocketOffset=InitialBoomOffset;
}

void AABChracter::CameraZoomInterp(float DeltaTime)
{
	//FOV	
	float TargetFOV = bIsBowAiming ? AimFOV : DefaultFOV;

	if (FMath::Abs(FollowCamera->FieldOfView - TargetFOV) > 0.1f)
	{
		float NewFOV = FMath::FInterpTo(
			FollowCamera->FieldOfView,
			TargetFOV,
			DeltaTime,
			10.f
		);
		
		FollowCamera->SetFieldOfView(NewFOV);
	}
	
	//Camera Socket Offset
	FVector TargetOffset = bIsBowAiming ? AimBoomOffset : InitialBoomOffset;

	if (!CameraBoom->SocketOffset.Equals(TargetOffset, 0.1f))
	{
		CameraBoom->SocketOffset = FMath::VInterpTo(
		CameraBoom->SocketOffset,
		TargetOffset,
		DeltaTime,
		10.f
		);
	}
}

void AABChracter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AABChracter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AABChracter::DoJumpStart()
{
	Jump();
}

void AABChracter::DoJumpEnd()
{
	StopJumping();
}
