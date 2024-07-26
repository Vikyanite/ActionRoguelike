// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// check the distance between the AI pawn and target actor
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor")))
		{
			if (AAIController* AIController = OwnerComp.GetAIOwner())
			{
				if (AActor* AIPawn = AIController->GetPawn())
				{
					float DistanceTo = FVector::Dist(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
					bool bWithinRange = DistanceTo <= 2000.f;
					bool bHasLOS = false;
					
					if (bWithinRange)
					{
						bHasLOS = AIController->LineOfSightTo(TargetActor);
					}
					
					BlackboardComp->SetValueAsBool(AttackRangerKey.SelectedKeyName, bWithinRange && bHasLOS);
				}
			}
		}
	}
}
