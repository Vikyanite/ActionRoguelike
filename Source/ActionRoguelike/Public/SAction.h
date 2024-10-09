// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SAction.generated.h"

class USActionComponent;
struct FGameplayTagContainer;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	USActionComponent* ActionComp;
	
	UFUNCTION(BlueprintCallable, Category="Action")
	USActionComponent* GetOwningComponent();
	
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();
	
public:
	void Initialize(USActionComponent* InActionComp);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const { return RepData.bIsRunning; }

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
