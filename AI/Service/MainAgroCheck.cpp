// Fill out your copyright notice in the Description page of Project Settings.

#include "MainAgroCheck.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Controller/MainAIController.h"
#include "LibertyPrimeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

UMainAgroCheck::UMainAgroCheck()
{
	bCreateNodeInstance = true;
}

void UMainAgroCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AMainAIController* AIConRef = Cast<AMainAIController>(OwnerComp.GetAIOwner());

	if (AIConRef)
	{
		ALibertyPrimeCharacter* AIOwner = Cast<ALibertyPrimeCharacter>(AIConRef->GetPawn());

		if (AIOwner)
		{
			ALibertyPrimeCharacter* BB_Target_Val = Cast<ALibertyPrimeCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(AIConRef->EnemyKeyID));

			if (BB_Target_Val && BB_Target_Val->IsDead)
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(AIConRef->EnemyKeyID, nullptr);
			}

			FVector TraceStart = AIOwner->GetActorLocation();
			FVector TraceEnd = TraceStart + FVector(0.0f, 0.0f, 15.0f);
			TArray<FHitResult> HitOuts;
			TArray<AActor*> FoundActors;
			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), FoundActors);

			for (auto FoundActor : FoundActors)
			{
				ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(FoundActor);

				if (IsLPC)
				{
					if (IsLPC->faction != AIOwner->faction)
					{
						FoundActors.Remove(IsLPC);
					}
				}
			}

			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActors(FoundActors);
			CollisionQueryParams.bTraceAsyncScene = true;
			float SphereRadius = 1500.0f;

			GetWorld()->SweepMultiByObjectType(HitOuts, TraceStart, TraceEnd, FQuat(), ObjectQueryParams, FCollisionShape::MakeSphere(SphereRadius), CollisionQueryParams);

			ALibertyPrimeCharacter* Target = NULL;
			bool IsTargetFound = false;
			float MinDistance = SphereRadius;

			for (auto HitOut : HitOuts)
			{
				ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());

				if (IsTarget && !IsTarget->IsDead)
				{
					float Distance = (AIOwner->GetActorLocation() - IsTarget->GetActorLocation()).Size();

					if (Distance < MinDistance)
					{
						FVector Forward = AIOwner->GetActorForwardVector();
						FVector Offset = (IsTarget->GetActorLocation() - TraceStart).GetSafeNormal();

						float DotProduct = FVector::DotProduct(Forward, Offset);

						if ((DotProduct >= 0.0f || (IsTarget->GetVelocity().Size() > (IsTarget->WalkSpeed + 10.0f)) && (Distance < SphereRadius / 1.8f)) || (AIOwner->DamagerApplier && AIOwner->DamagerApplier == IsTarget) || (!bBuildUp && AIOwner->bBuildUp))
						{
							ECollisionChannel TraceChannel = ECollisionChannel::ECC_Visibility;
							TraceEnd = IsTarget->GetActorLocation();
							FHitResult hitout;
							FCollisionResponseParams ResponseParams;
							ResponseParams.CollisionResponse.Visibility;
							GetWorld()->LineTraceSingleByChannel(hitout, TraceStart, TraceEnd, TraceChannel, CollisionQueryParams, ResponseParams);

							if (hitout.GetActor() == IsTarget)
							{
								MinDistance = Distance;
								Target = IsTarget;
								IsTargetFound = true;

								if (!bBuildUp && AIOwner->bBuildUp)
								{
									bBuildUp = true;
									AIOwner->Build_Up();
								}
							}
						}
					}
				}
			}

			if (IsTargetFound)
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(AIConRef->EnemyKeyID, Target);
			}
		}
	}
}