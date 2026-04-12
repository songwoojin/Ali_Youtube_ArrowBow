// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/ABBow.h"

AABBow::AABBow()
	:CurrentBowState(EBowState::Idle)
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

