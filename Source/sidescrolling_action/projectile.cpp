// Fill out your copyright notice in the Description page of Project Settings.


#include "projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
bool alive = true;

//allows me to use print() for debuging
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, TEXT(text))

// Sets default values
Aprojectile::Aprojectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void Aprojectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(1);
}

// Called every frame
void Aprojectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

