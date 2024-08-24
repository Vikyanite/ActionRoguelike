// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

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
