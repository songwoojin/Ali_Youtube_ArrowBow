// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ABBowMechanicsComponent.h"
#include "Actor/ABBow.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/ABArrow.h"
#include "Camera/CameraComponent.h"
#include "Character/ABChracter.h"

UABBowMechanicsComponent::UABBowMechanicsComponent()
	:BowClass(nullptr)
	,Bow(nullptr)
	,Character(nullptr)
	,bIsAiming(false)
{
	PrimaryComponentTick.bCanEverTick = false;

	BowSocketName=FName("bow_socket");
	ArrowSocketName=FName("arrow_socket");
}

void UABBowMechanicsComponent::BeginPlay()
{
	Super::BeginPlay();

	Character=Cast<ACharacter>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("Character: %s"), *GetNameSafe(Character));
	EquipBow();
	InitRotationRate();
}

void UABBowMechanicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UABBowMechanicsComponent::SpawnBow()
{
	if (BowClass)
	{
		Bow=Cast<AABBow>(GetWorld()->SpawnActor(BowClass));
		Bow->SetOwner(Character);
		if (Bow)
		{
			if (Character)
			{
				Bow->AttachToComponent(Character->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,BowSocketName);
			}
			
			//Test
			//Bow->AttachToActor(GetOwner(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,BowSocketName);
		}
	}
}

void UABBowMechanicsComponent::EquipBow()
{
	SpawnBow();
}

void UABBowMechanicsComponent::InitRotationRate()
{
	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
	if (MoveComp)
	{
		InitialRotationRate=MoveComp->RotationRate;
		AimRotationRate=FRotator(0.0f,1000.0f,0.0f);
	}
}

void UABBowMechanicsComponent::SpawnArrow()
{
	if (ArrowClass)
	{
		Arrow=Cast<AABArrow>(GetWorld()->SpawnActor(ArrowClass));
		//Arrow->SetOwner(Character);
		if (Arrow)
		{
			if (Character)
			{
				Arrow->AttachToComponent(Character->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,ArrowSocketName);
			}
		}
	}
}

void UABBowMechanicsComponent::DestroyArrow()
{
	if (IsValid(Arrow))
	{
		Arrow->Destroy();
	}
}

void UABBowMechanicsComponent::FireArrowBegin()
{
	if (IsValid(Arrow))
	{
		Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AABChracter* ABCharacter=Cast<AABChracter>(Character);
		if (ABCharacter)
		{
			FVector Direction=ABCharacter->GetFollowCamera()->GetForwardVector();
			Arrow->Fire(Direction);
		}
	}
}

void UABBowMechanicsComponent::AimBegin()
{
	bIsAiming=true;

	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->bOrientRotationToMovement=false;
		MoveComp->bUseControllerDesiredRotation=true;
		MoveComp->RotationRate=AimRotationRate;
	}

	Bow->SetBowState(EBowState::Aim);
	SpawnArrow();

}

void UABBowMechanicsComponent::AimEnd()
{
	bIsAiming=false;
	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->bOrientRotationToMovement=true;
		MoveComp->bUseControllerDesiredRotation=false;
		MoveComp->RotationRate=InitialRotationRate;
	}
	
	Bow->SetBowState(EBowState::Idle);
	DestroyArrow();
}

