// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"


// Sets default values
ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASPowerup_HealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerup_HealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* Player = Cast<ASCharacter>(InstigatorPawn))
	{
		USAttributeComponent* AttrComp = Player->GetAttributeComponent();
		if (AttrComp->ApplyHealthChange(AttrComp->HealthMax))
		{
			HideAndCooldown();
		}
	}
}

