// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ABBowTypes.h"
#include "ABBow.generated.h"


UCLASS()
class ALI_ARROWBOW_API AABBow : public AActor
{
	GENERATED_BODY()
	
public:	
	AABBow();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetBowState(EBowState NewState){CurrentBowState=NewState;}
	float GetMaxDrawTime() const {return MaxDrawTime;}
	void DrawBegin();
	void DrawEnd();

protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(BlueprintReadOnly)
	EBowState CurrentBowState;

	UPROPERTY(EditAnywhere,category=Settings)
	float MaxDrawTime;

	UPROPERTY(EditAnywhere,category=Settings)
	USoundBase* DrawSound;

	UPROPERTY()
	UAudioComponent* AudioComp;
};
