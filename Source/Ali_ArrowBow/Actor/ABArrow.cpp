// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ABArrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DataWrappers/ChaosVDQueryDataWrappers.h"

AABArrow::AABArrow()
	:MinSpeed(1000.0f)
	,MaxSpeed(3000.0f)
	,MinGravity(0.0f)
	,MaxGravity(0.5f)
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent=CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//BoxComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	//BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	RootComponent=BoxComponent;

	SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	SkeletalMesh->SetupAttachment(BoxComponent);

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(RootComponent);
	ProjectileMovement->ProjectileGravityScale=0.0f;
	ProjectileMovement->bRotationFollowsVelocity=true;
	ProjectileMovement->Velocity=FVector(0.0f,0.0f,0.0f);

	TrailEffectLocation=CreateDefaultSubobject<USceneComponent>(TEXT("TrailEffectLocation"));
	TrailEffectLocation->SetupAttachment(SkeletalMesh);
}

void AABArrow::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AABArrow::OnBoxCollisionOverlapBegin);
}

void AABArrow::OnBoxCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor==GetInstigator())	return;
	
	if (IsValid(ProjectileMovement))
	{
		ProjectileMovement->DestroyComponent(false);
	}

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	USceneComponent* HitComponent= SweepResult.GetComponent();
	
	AttachToComponent(HitComponent,FAttachmentTransformRules::KeepWorldTransform);
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		ArrowHitSound,
		GetActorLocation()
	);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	GetWorld(),
	HitImpactEffect,                      // UNiagaraSystem*
	SweepResult.ImpactPoint,               // 충돌 위치
	SweepResult.ImpactNormal.Rotation()    // 표면 방향
);

	if (HitComponent->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(Velocity,FName("None"),true);
	}

	if (TrailComponent)
	{
		TrailComponent->DestroyComponent(false);
	}
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
	Velocity=Direction*Speed;
	//UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *Velocity.ToString());
	ProjectileMovement->Velocity=Velocity;
	ProjectileMovement->ProjectileGravityScale=GravityScale;
	ProjectileMovement->Activate();

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		ArrowWhooshSound,
		GetActorLocation()
	);

	TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
	TrailEffect,
	TrailEffectLocation,
	FName("None"),
	FVector::ZeroVector,
	FRotator::ZeroRotator,
	EAttachLocation::KeepRelativeOffset,
	true
);

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SetLifeSpan(5.0f);
	
}

