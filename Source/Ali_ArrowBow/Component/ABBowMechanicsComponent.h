// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABBowMechanicsComponent.generated.h"


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
	
	void EquipBow();
	void InitRotationRate();
	
	UFUNCTION(BlueprintCallable)
	void AimBegin();
	
	UFUNCTION(BlueprintCallable)
	void AimEnd();
	
protected:
	UPROPERTY(EditAnywhere,category=Settings)
	TSubclassOf<AActor> BowClass;

	UPROPERTY()
	AActor* Bow;

	UPROPERTY(EditAnywhere,category=Settings)
	FName BowSocketName;

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming;

	FRotator AimRotationRate;
	FRotator InitialRotationRate;
};
