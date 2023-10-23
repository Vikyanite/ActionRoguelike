// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Public/ExplosiveBarrel.h"

#include "ActionRoguelike/Public/MagicProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->Radius = 600.f;
	RadialForceComp->ImpulseStrength = 2000.f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->SetupAttachment(MeshComp);


	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetGenerateOverlapEvents(true);

	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::HitHandler);
	
}

void AExplosiveBarrel::HitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit!"));

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

