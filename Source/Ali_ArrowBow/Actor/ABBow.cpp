// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/ABBow.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AABBow::AABBow()
	:CurrentBowState(EBowState::Idle)
	,MaxDrawTime(1.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
}

void AABBow::BeginPlay()
{
	Super::BeginPlay();
}

void AABBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABBow::DrawBegin()
{
	CurrentBowState=EBowState::Draw;

	AudioComp = UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(),
		DrawSound,          
		GetActorLocation()  
	);
}

void AABBow::DrawEnd()
{
	CurrentBowState=EBowState::Idle;

	if (IsValid(AudioComp))
	{
		AudioComp->Stop();
	}
}

