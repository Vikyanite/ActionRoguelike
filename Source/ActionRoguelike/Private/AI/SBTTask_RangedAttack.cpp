﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	if (!USAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
	if (!AICharacter)
	{
		return EBTNodeResult::Failed;
	}
	FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator MuzzleRotation = (TargetActor->GetActorLocation() - MuzzleLocation).Rotation();

	MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpread);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = AICharacter;

	AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params); 

	return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	MaxBulletSpread = 2.f;	
}
