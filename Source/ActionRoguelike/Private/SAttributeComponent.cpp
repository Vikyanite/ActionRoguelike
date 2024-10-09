// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Public/SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("su.DamageMultiplier"),
	1.f,
	TEXT("Global damage multiplier."),
	ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;

	RageMax = 100.f;
	Rage = 0.f;

	SetIsReplicatedByDefault(true);
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
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false;
	}
	
	if (Delta < 0.f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}
	
	float PreHealth = Health;
	float CurHealth = FMath::Clamp(Health+Delta, 0.f, HealthMax);
	
	float ActualDelta = CurHealth - PreHealth;

	//Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = CurHealth;
		
		if (ActualDelta != 0.f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// Died
		if (ActualDelta < 0.f && Health == 0.f)
		{
			if (ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>())
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0.f;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float PreRage = Rage;
	Rage = FMath::Clamp(Rage+Delta, 0.f, RageMax);

	float ActualDelta = Rage-PreRage;
	if (ActualDelta != 0.f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}
	
	return ActualDelta != 0.f;
}

bool USAttributeComponent::IsFullHealth()
{
	return Health == HealthMax;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
	// DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_InitialOnly);
}

