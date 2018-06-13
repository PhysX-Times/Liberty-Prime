// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ALibertyPrimeCharacter;

UCLASS()
class LIBERTYPRIME_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	class ALibertyPrimeCharacter* MyOwner;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		virtual void Clear_PS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
