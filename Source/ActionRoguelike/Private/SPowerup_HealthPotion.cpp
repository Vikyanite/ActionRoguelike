// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"


// Sets default values
ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreditCost = 50;
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	if (ASCharacter* Player = Cast<ASCharacter>(InstigatorPawn))
	{
		USAttributeComponent* AttrComp = Player->GetAttributeComponent();
		if (ensure(AttrComp) && !AttrComp->IsFullHealth())
		{
			if (ASPlayerState* PS = Player->GetPlayerState<ASPlayerState>())
			{
				if (PS->RemoveCredits(CreditCost) && AttrComp->ApplyHealthChange(this, AttrComp->HealthMax))
				{
					HideAndCooldown();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Not enough credits or health is full"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("full health interation skipped"));
		}
	}
}

