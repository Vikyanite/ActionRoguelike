// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "GameFramework/Actor.h"
#include "MagicProjectile.generated.h"

struct FGameplayTag;
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API AMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMagicProjectile();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag ParryTag;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
