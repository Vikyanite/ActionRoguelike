// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta>0))
	{
		return ;
	}
	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ASPlayerState::RemoveCredits(int32 delta)
{
	ensure(delta>0);
	if (Credits < delta)
	{
		return false;
	}
	
	Credits -= delta;

	OnCreditsChanged.Broadcast(this, Credits, delta);

	return true;
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits-OldCredits);
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveGame)
{
	if (SaveGame)
	{
		SaveGame->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveGame)
{
	if (SaveGame)
	{
		AddCredits(SaveGame->Credits);
	}
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}