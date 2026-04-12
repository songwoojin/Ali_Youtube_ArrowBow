// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ABArrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AABArrow::AABArrow()
	:Speed(1000.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent=CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent=BoxComponent;

	SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	SkeletalMesh->SetupAttachment(BoxComponent);

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(RootComponent);
	ProjectileMovement->ProjectileGravityScale=0.5f;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
}

void AABArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AABArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABArrow::Fire(FVector Direction)
{
	FVector Velocity=Direction*Speed;
	UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *Velocity.ToString());

	//UE_LOG(LogTemp,Warning,TEXT("Arrow Speed : %d"),)
	ProjectileMovement->Velocity=Velocity;
	ProjectileMovement->Activate();	
}

