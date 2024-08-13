// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Public/SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Health = 100.f;
}

USAttributeComponent* USAttributeComponent::GetAttributeComp(const AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(const AActor* FromActor)
{
	if (USAttributeComponent* AttrComp = GetAttributeComp(FromActor))
	{
		return AttrComp->IsAlive();
	}
	return false;
}

bool USAttributeComponent::IsAlive()
{
	return Health > 0.f;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	float PreHealth = Health;
	Health = FMath::Clamp(Health+Delta, 0.f, HealthMax);

	float ActualDelta = Health-PreHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	return ActualDelta != 0.f;
}

