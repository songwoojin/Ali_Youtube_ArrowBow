// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ABBowMechanicsComponent.h"
#include "GameFramework/Character.h"

UABBowMechanicsComponent::UABBowMechanicsComponent()
	:BowClass(nullptr)
	,Bow(nullptr)
	,Character(nullptr)
	,bIsAimging(false)
{
	PrimaryComponentTick.bCanEverTick = false;

	BowSocketName=FName("bow_socket");
}

void UABBowMechanicsComponent::BeginPlay()
{
	Super::BeginPlay();

	EquipBow();
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
		if (Bow)
		{
			Character=Cast<ACharacter>(GetOwner());
			if (Character)
			{
				Bow->AttachToComponent(Character->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,BowSocketName);
			}
			
			//Test
			//Bow->AttachToActor(GetOwner(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,BowSocketName);
		}
	}
}

void UABBowMechanicsComponent::AimBegin()
{
	bIsAimging=true;
}

void UABBowMechanicsComponent::AimEnd()
{
	bIsAimging=false;
}

