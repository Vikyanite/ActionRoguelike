// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.f;
	
	bIsActive = true;

	bReplicates = true;
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

void ASPowerupActor::Show()
{
	SetPowerupState(true);
}

void ASPowerupActor::HideAndCooldown()
{
	SetPowerupState(false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::Show, RespawnTime);
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsActive);
}

void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}


