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
	,bIsDrawingBow(false)
	,DrawTime(0.0)
	,DrawIncrementTime(0.333333)
	,bIsFiringBow(false)
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
		Arrow=nullptr;
	}
}

void UABBowMechanicsComponent::FireAimedArrow()
{
	if (IsValid(Arrow))
	{
		Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AABChracter* ABCharacter=Cast<AABChracter>(Character);
		if (ABCharacter)
		{
			FVector Direction=ABCharacter->GetFollowCamera()->GetForwardVector();
			//UE_LOG(LogTemp, Warning, TEXT("Dir: %s"), *Direction.ToString());

			float Strength=FMath::GetRangePct(0.0f,Bow->GetMaxDrawTime(),DrawTime);
			Strength = FMath::Clamp(Strength,-1.0f,1.0f);
			Arrow->Fire(Direction,Strength);
			Arrow=nullptr;
		}
	}
}

void UABBowMechanicsComponent::FireArrowBegin()
{
	if (!bIsDrawingBow)	return;

	//짧게 눌렀을 경우 제대로 날아가지 않고 남아있는 버그 발생
	if (!IsValid(Arrow))	return;
	
	bIsFiringBow=true;
	FireAimedArrow();
	DrawEnd();

	//원래는 애니메이션 끝날때 실행되도록 해야하지만 임시로 1초후 끝내도록
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&UABBowMechanicsComponent::FireArrowEnd,
		1.0f,   // 시간 (초)
		false
	);// 반복 여부
}

void UABBowMechanicsComponent::FireArrowEnd()
{
	bIsFiringBow=false;

	if (!bIsAiming)	return;

	AimBegin();
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
	DrawEnd();
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

void UABBowMechanicsComponent::DrawBegin()
{
	if (!bIsDrawingBow && bIsAiming)
	{
		bIsDrawingBow=true;

		GetWorld()->GetTimerManager().SetTimer(
		DrawTimerHandle,
		this,
		&UABBowMechanicsComponent::IncrementDrawTime,
		DrawIncrementTime,   // 시간 (초)
		true   // 반복 여부
		);

		Bow->SetBowState(EBowState::Draw);
	}
}

void UABBowMechanicsComponent::DrawEnd()
{
	bIsDrawingBow=false;
	DrawTime=0.0f;
	GetWorld()->GetTimerManager().ClearTimer(DrawTimerHandle);
	DrawTimerHandle.Invalidate();
	Bow->SetBowState(EBowState::Idle);
}

void UABBowMechanicsComponent::IncrementDrawTime()
{
	DrawTime+=DrawIncrementTime;
	UE_LOG(LogTemp,Log,TEXT("DrawTime: %f"),DrawTime);
}

