// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChangedSignature, ASPlayerState*, PlayerState, int32,
                                               NewCredits, int32, Delta);
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category="Credits")
	int32 Credits=0;
	
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

public:
	UFUNCTION(BlueprintCallable, Category="Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Credits")
	int32 GetCredits() const { return Credits; }

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnCreditsChangedSignature OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveGame);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveGame);
};
