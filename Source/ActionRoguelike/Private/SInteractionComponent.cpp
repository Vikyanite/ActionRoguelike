// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Public/SInteractionComponent.h"

#include "InputBehavior.h"
#include "ActionRoguelike/Public/SGameplayInterface.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("su.DebugDraw"),
	false,
	TEXT("Enable interaction debug drawing"),
	ECVF_Cheat
);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.f);

	TArray<FHitResult> Hits;

	float Radius = 30.f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockHit ? FColor::Green : FColor::Red;
	for (FHitResult Hit : Hits)
	{
		if (CVarDebugDrawInteraction.GetValueOnGameThread())
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.f);
		}
		
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}
	if (CVarDebugDrawInteraction.GetValueOnGameThread())
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f);
	}
}