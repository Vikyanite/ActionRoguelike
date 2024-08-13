// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(Pawn);
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(Pawn, AttributeComp->HealthMax);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
