// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABBowMechanicsComponent.generated.h"

class AABBow;
class AABArrow;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALI_ARROWBOW_API UABBowMechanicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UABBowMechanicsComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void AimBegin();
	
	UFUNCTION(BlueprintCallable)
	void AimEnd();

	void DrawBegin();
	void FireArrowBegin();
protected:
	//Bow
	void SpawnBow();
	void EquipBow();
	void InitRotationRate();
	
	//Arrow
	void FireArrowEnd();
	void SpawnArrow();
	void DestroyArrow();
	void FireAimedArrow();
	
	void DrawEnd();
	void IncrementDrawTime();
	
protected:
	//Bow
	UPROPERTY(EditAnywhere,category=Settings)
	TSubclassOf<AActor> BowClass;

	UPROPERTY()
	AABBow* Bow;

	UPROPERTY(EditAnywhere,category=Settings)
	FName BowSocketName;

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming;

	FRotator AimRotationRate;
	FRotator InitialRotationRate;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDrawingBow;
	FTimerHandle DrawTimerHandle;
	float DrawTime;
	float DrawIncrementTime;

	bool bIsFiringBow;
	
	//Arrow
	UPROPERTY(EditAnywhere,category=Settings)
	TSubclassOf<AActor> ArrowClass;

	UPROPERTY()
	AABArrow* Arrow;

	UPROPERTY(EditAnywhere,category=Settings)
	FName ArrowSocketName;
	
};
