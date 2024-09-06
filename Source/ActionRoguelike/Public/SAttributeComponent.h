// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category="Attributes")
	static USAttributeComponent* GetAttributeComp(const AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(DisplayName="IsAlive"))
	static bool IsActorAlive(const AActor* FromActor);

public:
	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnRageChanged OnRageChanged;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float HealthMax = 100.f;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float RageMax = 100.f;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsFullHealth();

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	float GetRage() const { return Rage; }
	
};
