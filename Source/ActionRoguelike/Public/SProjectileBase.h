// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USoundCue;
class UParticleSystem;
class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UParticleSystemComponent* EffectComp;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
		const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;

	// audio
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundCue* ImpactSound;
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
