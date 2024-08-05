// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Public/MagicProjectile.h"

#include "ActionRoguelike/Public/SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMagicProjectile::AMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OverlapHandler);
	DamageAmount = 20.f;
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagicProjectile::OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-DamageAmount);
			Explode();
		}
	}
}

// Called every frame
void AMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

