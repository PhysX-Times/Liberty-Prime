// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Reaper.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AReaper : public AMonster
{
	GENERATED_BODY()
	
public:

	AReaper();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;

	bool bSwitcher;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	bool bFuck;
	bool bCanDuplicate;

	int SpawnCount;

	AReaper* Reaper_Twin;

	FVector Spawn_InitialLoc;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* MagicAttackA_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* MagicAttackB_Montage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* MagicB_Cue;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ScytheDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<class AReaper> ReaperClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* ScytheDamager;

	UPROPERTY()
		UAttackData* MagicAttackData_A;
	UPROPERTY()
		UAttackData* MagicAttackData_B;

	void MagicAttack_A();
	void MagicAttack_B();

	FTimerHandle GlitchTimer;

	FTimeline GlitchLine;

	void Spawn_Twin();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void Twin_Reset();

	void Glitch();
	UFUNCTION()
		void GlitchFunction(float Val);

	UPROPERTY()
		UCurveFloat* GlitchCurveFloat;

	void Twin_Play();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ProjectilePoint, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ProjectilePoint_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ProjectilePoint, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ProjectilePoint_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ProjectilePoint, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ProjectilePoint_3;

};
