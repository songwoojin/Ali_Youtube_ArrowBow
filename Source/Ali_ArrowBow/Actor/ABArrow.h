// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABArrow.generated.h"

class UNiagaraComponent;
class UBoxComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class ALI_ARROWBOW_API AABArrow : public AActor
{
	GENERATED_BODY()
	
public:	

	AABArrow();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxCollisionOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:	
	virtual void Tick(float DeltaTime) override;

	void Fire(FVector Direction,float Strength);

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere,Category="Settings")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere,Category="Settings")
	float MinSpeed;

	UPROPERTY(EditAnywhere,Category="Settings")
	float MaxSpeed;

	UPROPERTY(EditAnywhere,Category="Settings")
	float MinGravity;

	UPROPERTY(EditAnywhere,Category="Settings")
	float MaxGravity;

	FVector Velocity;
	
	UPROPERTY(EditAnywhere,Category="Settings")
	USoundBase* ArrowWhooshSound;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* TrailEffectLocation;

	UPROPERTY(EditAnywhere,Category="Settings")
	UNiagaraSystem* TrailEffect;

	UPROPERTY()
	UNiagaraComponent* TrailComponent;
	
	UPROPERTY(EditAnywhere,Category="Settings")
	USoundBase* ArrowHitSound;

	UPROPERTY(EditAnywhere,Category="Settings")
	UNiagaraSystem* HitImpactEffect;
};
