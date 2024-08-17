// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USActionComponent;
class USAttributeComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName TimeToHitParamName = "TimeToHit";
	
	void virtual PostInitializeComponents() override;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USInteractionComponent* InteractionComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USActionComponent* ActionComp;

	void MoveForward(float Value);

	void MoveRight(float Value);
	
	void PrimaryAttack();

	void PrimaryInteract();

	void StartSprint();

	void StopSprint();

	void BlackHoleAttack();

	void Dash();
	
	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USAttributeComponent* GetAttributeComponent() const { return AttributeComp; }

	UFUNCTION(Exec)
	void HealSelf(float Amount=100);
	
};
