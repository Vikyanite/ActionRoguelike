// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UBehaviorTree* BehaviorTree;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
