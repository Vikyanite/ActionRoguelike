// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category="VFX")
	UParticleSystem* CastSpellVFX;

	UPROPERTY(EditAnywhere, Category="Attack")
	FName TimeToHitParamName;

	UPROPERTY(EditAnywhere, Category="Attack")
	FName HandSocketName;

	UFUNCTION()
	void AttackDelay_TimeElapsed(ACharacter* Character);
	
public:
	USAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
