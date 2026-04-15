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
	,bIsFiringArrow(false)
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

void UABBowMechanicsComponent::OnFireBowMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != FireBowMontage) return;
	
	FireArrowEnd();
}

void UABBowMechanicsComponent::SpawnArrow()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnArrow"));

	//Test Bug
	if (IsValid(Arrow)) return;
	
	if (ArrowClass)
	{
		Arrow=Cast<AABArrow>(GetWorld()->SpawnActor(ArrowClass));
		Arrow->SetInstigator(Character);
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
	UE_LOG(LogTemp, Warning, TEXT("FireAimedArrow"));
	
	if (IsValid(Arrow))
	{
		Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//UE_LOG(LogTemp,Warning,TEXT("Arrow Detach"));
		AABChracter* ABCharacter=Cast<AABChracter>(Character);
		if (ABCharacter)
		{
			FVector Direction=CalculateAimDirection();
			//UE_LOG(LogTemp, Warning, TEXT("Dir: %s"), *Direction.ToString());

			float Strength=FMath::GetRangePct(0.0f,Bow->GetMaxDrawTime(),DrawTime);
			Strength = FMath::Clamp(Strength,-1.0f,1.0f);
			Arrow->Fire(Direction,Strength);

			//TestBug
			Arrow=nullptr;
		}
	}
}

FVector UABBowMechanicsComponent::CalculateAimDirection()
{
	AABChracter* ABCharacter=Cast<AABChracter>(Character);
	if (!ABCharacter)	return FVector::ZeroVector;

	FVector CameraLocation=ABCharacter->GetFollowCamera()->GetComponentLocation();
	FVector CameraDirection=ABCharacter->GetFollowCamera()->GetForwardVector();
	const float Distance=10000.0f;
	FVector TargetLocation=CameraLocation+CameraDirection*Distance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(ABCharacter);
	bool bHit=GetWorld()->LineTraceSingleByChannel(Hit,CameraLocation,TargetLocation,ECollisionChannel::ECC_Visibility,Params);

	if (bHit)
	{
		TargetLocation=Hit.Location;
	}

	FVector StartLocation=FVector(FVector::ZeroVector);
	if (IsValid(Arrow))
	{
		StartLocation=Arrow->GetActorLocation();
	}

	FVector Direction=(TargetLocation-StartLocation).GetSafeNormal();
	
	// NOTE::Debug Draw
	// DrawDebugLine(
	// 	GetWorld(),
	// 	StartLocation,
	// 	TargetLocation,
	// 	bHit ? FColor::Red : FColor::Green,
	// 	false,
	// 	1.0f,
	// 	0,
	// 	1.0f
	// );

	return Direction;
}

void UABBowMechanicsComponent::FireArrowBegin()
{
	if (!bIsDrawingBow)	return;
	if (bIsFiringArrow) return;
	
	bIsFiringArrow=true;
	FireAimedArrow();
	DrawEnd();
	
	if (FireBowMontage)
	{
		UAnimInstance* AnimInstance=Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this,&UABBowMechanicsComponent::OnFireBowMontageEnded);
			AnimInstance->OnMontageEnded.AddDynamic(this,&UABBowMechanicsComponent::OnFireBowMontageEnded);
			AnimInstance->Montage_Play(FireBowMontage);
		}
	}
}

void UABBowMechanicsComponent::FireArrowEnd()
{
	bIsFiringArrow=false;

	if (!bIsAiming)	return;

	//Test Bug
	//Arrow=nullptr;
	
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

	OnAimBegin.Broadcast();

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
	OnAimEnd.Broadcast();
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

		Bow->DrawBegin();
	}
}

void UABBowMechanicsComponent::DrawEnd()
{
	bIsDrawingBow=false;
	DrawTime=0.0f;
	GetWorld()->GetTimerManager().ClearTimer(DrawTimerHandle);
	DrawTimerHandle.Invalidate();
	Bow->SetBowState(EBowState::Idle);
	Bow->DrawEnd();
	OnDrawEnd.Broadcast();
}

void UABBowMechanicsComponent::IncrementDrawTime()
{
	DrawTime+=DrawIncrementTime;
	//UE_LOG(LogTemp,Log,TEXT("DrawTime: %f"),DrawTime);

	OnDrawOnGoing.Broadcast(Bow->GetMaxDrawTime(),DrawTime);
}

