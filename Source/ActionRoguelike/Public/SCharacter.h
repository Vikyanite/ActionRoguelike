// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandler_PrimaryAttack;
	void PrimaryAttack_TimeElapsed();

	
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> DashProjectileClass;
	
	FTimerHandle TimerHandler_Dash;
	void Dash_TimeElapsed();


	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;
	
	FTimerHandle TimerHandler_BlackHoleAttack;
	void BlackHoleAttack_TimeElapsed();
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName TimeToHitParamName = "TimeToHit";
	
	void virtual PostInitializeComponents() override;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USAttributeComponent* AttributeComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float AttackAnimDelay;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);
	
	void PrimaryAttack();

	void Jump();

	void PrimaryInteract();

	void BlackHoleAttack();

	void Dash();

	void SpawnProjectile(TSubclassOf<AActor> ClassSpawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
