// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_Credits.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Credits : public ASPowerupActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPowerup_Credits();

protected:
	
	UPROPERTY(EditAnywhere, Category="Credits")
	int CreditsAmount;

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
