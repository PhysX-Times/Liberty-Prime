// Fill out your copyright notice in the Description page of Project Settings.

#include "Decorator.h"
#include "Components/StaticMeshComponent.h"
#include "LibertyPrimeCharacter.h"

ADecorator::ADecorator()
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Decorator_ST = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Decorator_ST"));
	Decorator_ST->SetupAttachment(RootComponent);

	Decorator_ST->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Decorator_ST->bGenerateOverlapEvents = false;
}

void ADecorator::BeginPlay()
{
	Super::BeginPlay();

	MyOwner->BodyParts_ST.Add(Decorator_ST);
}