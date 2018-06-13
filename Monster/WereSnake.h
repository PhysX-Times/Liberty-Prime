// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "WereSnake.generated.h"

class ASword;
class ABlow;
class UAttackData;
class AIndicatorDecal;

UCLASS()
class LIBERTYPRIME_API AWereSnake : public AMonster
{
	GENERATED_BODY()
	
public:

	AWereSnake();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Long;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Close;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ABlow> BlowDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;

	UPROPERTY()
		UAttackData* SpinAttackData;
	UPROPERTY()
		UAttackData* JumpAttackData;
	UPROPERTY()
		UAttackData* SpitAttackData;

	void SpinAttack();
	void JumpAttack();
	void SpitVenom();
	UFUNCTION(BlueprintCallable, Category = "CppFunction")
		void Spit_notify();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
};
