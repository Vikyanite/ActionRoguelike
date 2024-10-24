// Copyright Epic Games, Inc. All Rights Reserved.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAdvancedTransformInputBox.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SPowerupActor.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable Bot Spawning"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;
	CreditsPerKill = 20;

	RequiredDistance = 2000;
	DesiredPowerupCount = 10;

	PlayerStateClass = ASPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}


void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(Bot);
		}
	}
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	
	if (ensure(PowerupClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupQueryCompleted);
		}
	}
	
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning is disabled"));
		return;
	}
	
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Nr of Alive Bots: %d"), NrOfAliveBots);

	float MaxBotCount = 10.f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max Bot Count Reached, Skip Spawning"));
		return ;
	}
	UEnvQueryInstanceBlueprintWrapper* QueryInstance  = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct,  nullptr);
	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn Bot EQS Query Failed"));
		return;
	}

	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if (SpawnLocations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(BotClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::OnPowerupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn Powerup EQS Query Failed"));
		return;
	}

	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocation;
	
	int32 SpawnCount = 0;
	
	while (SpawnCount < DesiredPowerupCount && SpawnLocations.Num()>0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, SpawnLocations.Num()-1);

		FVector PickedLocation = SpawnLocations[RandomLocationIndex];
		SpawnLocations.RemoveAt(RandomLocationIndex);

		bool bValidLocation = true;
		for (auto &OtherLocation : UsedLocation)
		{
			if ((PickedLocation-OtherLocation).Size() < RequiredDistance)
			{
				bValidLocation = false;
				break;
			}
		}

		if (!bValidLocation)
		{
			continue;
		}

		int32 RandomPowerupIndex = FMath::RandRange(0, PowerupClasses.Num()-1);
		
		GetWorld()->SpawnActor<AActor>(PowerupClasses[RandomPowerupIndex], PickedLocation, FRotator::ZeroRotator);

		UsedLocation.Add(PickedLocation);
		SpawnCount++;
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	if (ASCharacter* Character = Cast<ASCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_Respawn;
		
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("RespawnPlayerElapsed"), Character->GetController());

		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, TimerDel, RespawnDelay, false);
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));

	if (APawn* KillerPawn = Cast<APawn>(KillerActor))
	{
		if (ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		APawn* PrePawn = Controller->GetPawn();
		if (PrePawn)
		{
			PrePawn->Destroy();
		}
		
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Load Save Game"));
			return ;
		}
		UE_LOG(LogTemp, Log, TEXT("Save Game Loaded"));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New Save Game"));
	}
}