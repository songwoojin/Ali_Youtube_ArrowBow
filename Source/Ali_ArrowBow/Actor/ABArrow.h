// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABArrow.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class ALI_ARROWBOW_API AABArrow : public AActor
{
	GENERATED_BODY()
	
public:	

	AABArrow();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Fire(FVector Direction);

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	float Speed;
};
