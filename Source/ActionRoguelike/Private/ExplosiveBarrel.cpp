// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Public/ExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::HitHandler);
	
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(MeshComp);

	RadialForceComp->SetAutoActivate(false);
	
	RadialForceComp->Radius = 750.f;
	RadialForceComp->ImpulseStrength = 2500.f;
	
	RadialForceComp->bImpulseVelChange = true;
	
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
	
}

void AExplosiveBarrel::HitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

