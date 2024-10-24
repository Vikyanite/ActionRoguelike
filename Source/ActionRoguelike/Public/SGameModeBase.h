// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SaveGame.h"
#include "SGameModeBase.generated.h"

class USSaveGame;
class ASPowerupActor;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;

	ASGameModeBase();

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION()
	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

protected:
	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	FString SlotName;
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnTimerInterval;

	FTimerHandle TimerHandle_SpawnBots;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> BotClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Credits")
	int CreditsPerKill;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	UEnvQuery* PowerupSpawnQuery;
	
	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	TArray<TSubclassOf<ASPowerupActor>> PowerupClasses;
	
	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	int DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	int RequiredDistance;

	UFUNCTION()
	void OnPowerupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
