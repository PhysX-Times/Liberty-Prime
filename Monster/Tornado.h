// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Tornado.generated.h"

class ASword;
class UAttackData;
class AWeeper;

UCLASS()
class LIBERTYPRIME_API ATornado : public AMonster
{
	GENERATED_BODY()
	
public:

	ATornado();

	ALibertyPrimeCharacter* TornadoTarget;

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Death_Delay;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Trace_Height;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Trace_Radius;

	TArray<AWeeper*> Casters;

	FTimerHandle DeathTimer;
	FTimerHandle ResetTornadoTimer;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ClawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> BiteDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightClawDamager;

	void Caster_Die(AWeeper* Caster);
	void Die_Function();
	
	void Reset_Tornado_Function();

	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TornadoPS, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* TornadoPS_Comp;

};
