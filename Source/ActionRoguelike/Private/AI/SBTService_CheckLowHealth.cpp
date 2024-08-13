// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckLowHealth::USBTService_CheckLowHealth()
{
	LowHealthThreshold = 0.3f;
}

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (USAttributeComponent* AttrComp = USAttributeComponent::GetAttributeComp(AIPawn))
		{
			bool bIsLowHealth = (AttrComp->Health/AttrComp->HealthMax) <= LowHealthThreshold;
			
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
		}
	}
	
}
