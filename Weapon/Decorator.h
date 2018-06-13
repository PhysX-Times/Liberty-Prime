// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Decorator.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ADecorator : public AWeapon
{
	GENERATED_BODY()
	
public:
	
	ADecorator();

protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Decorator_ST, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Decorator_ST;
	
};
