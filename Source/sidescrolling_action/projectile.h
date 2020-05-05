// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "projectile.generated.h"

UCLASS()
class SIDESCROLLING_ACTION_API Aprojectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aprojectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
