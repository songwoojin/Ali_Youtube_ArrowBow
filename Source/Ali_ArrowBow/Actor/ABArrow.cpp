// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ABArrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AABArrow::AABArrow()
	:MinSpeed(1000.0f)
	,MaxSpeed(3000.0f)
	,MinGravity(0.0f)
	,MaxGravity(0.5f)
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
	ProjectileMovement->ProjectileGravityScale=0.0f;
	ProjectileMovement->bRotationFollowsVelocity=true;
	ProjectileMovement->Velocity=FVector(0.0f,0.0f,0.0f);
}

void AABArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AABArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABArrow::Fire(FVector Direction,float Strength)
{
	float Speed=FMath::Lerp(MinSpeed,MaxSpeed,Strength);
	UE_LOG(LogTemp,Warning,TEXT("Arrow Speed: %f"),Speed);
	float GravityScale = ProjectileMovement->ProjectileGravityScale=FMath::Lerp(MaxGravity,MinGravity,Strength);
	FVector Velocity=Direction*Speed;
	//UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *Velocity.ToString());
	ProjectileMovement->Velocity=Velocity;
	ProjectileMovement->ProjectileGravityScale=GravityScale;
	ProjectileMovement->Activate();	
}

