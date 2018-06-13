// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Weeper.generated.h"

class ASword;
class ATornado;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AWeeper : public AMonster
{
	GENERATED_BODY()
	
public:

	AWeeper();

	ATornado* Tornado_Dedicator;

	ALibertyPrimeCharacter* Protecter;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		FLinearColor CastPS_Color;

	UPROPERTY(EditAnywhere, Category = "Particles")
		TArray<UParticleSystem*> Cast_Particles;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Cast1_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Cast2_Montage;

	UPROPERTY()
		UAttackData* CastAttackData;

	UPROPERTY()
		UParticleSystemComponent* CastParticle_Comp;

	void CastAttack();

	void Cast_1();
	void Cast_2();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Cast_Particle(FName SocketName, bool bTraceCheck);

	virtual void BeginPlay() override;

	void Die_Add() override;
	
};
