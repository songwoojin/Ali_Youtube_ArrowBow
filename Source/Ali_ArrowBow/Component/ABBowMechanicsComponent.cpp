// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ABBowMechanicsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UABBowMechanicsComponent::UABBowMechanicsComponent()
	:BowClass(nullptr)
	,Bow(nullptr)
	,Character(nullptr)
	,bIsAiming(false)
{
	PrimaryComponentTick.bCanEverTick = false;

	BowSocketName=FName("bow_socket");
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

void UABBowMechanicsComponent::EquipBow()
{
	if (BowClass)
	{
		Bow=GetWorld()->SpawnActor(BowClass);
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

void UABBowMechanicsComponent::InitRotationRate()
{
	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
	if (MoveComp)
	{
		InitialRotationRate=MoveComp->RotationRate;
		AimRotationRate=FRotator(0.0f,1000.0f,0.0f);
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
}

