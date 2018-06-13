// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMusicManager::AMusicManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Volume = 1.0f;
}

// Called when the game starts or when spawned
void AMusicManager::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::SpawnSound2D(this, BackGroundMusic, Volume);
}

// Called every frame
void AMusicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

