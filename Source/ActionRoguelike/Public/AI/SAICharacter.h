// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
class UUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName TimeToHitParamName;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	void SetTargetActor(AActor* NewTarget);

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	USWorldUserWidget* ActiveHealthBar;
};
