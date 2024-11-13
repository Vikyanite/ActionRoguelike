// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	// UE_LOG(LogTemp, Warning, TEXT("[%s]: %s started"), Comp->GetOwner()->HasAuthority() ? TEXT("Server"):TEXT("Client"), *ActionName.ToString());

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwner()->HasAuthority())
	{
		TimeStarted = GetWorld()->GetTimeSeconds();
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::Initialize(USActionComponent* InActionComp)
{
	ActionComp = InActionComp;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	// UE_LOG(LogTemp, Warning, TEXT("[%s]: %s stopped"), Comp->GetOwner()->HasAuthority() ? TEXT("Server"):TEXT("Client"), *ActionName.ToString());
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

USActionComponent* USAction::GetOwningComponent()
{
	// AActor* Actor = Cast<AActor>(GetOuter());
	// return Actor->FindComponentByClass<USActionComponent>();

	return ActionComp;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	if (USActionComponent* ActComp = GetOwningComponent())
	{
		if (ActComp->ActiveGameplayTags.HasAny(BlockedTags))
		{
			return false;
		}
	}
	return true;
}

UWorld* USAction::GetWorld() const
{
	if (AActor* Actor = Cast<AActor>(GetOuter()))
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, TimeStarted);
	DOREPLIFETIME(USAction, ActionComp);
}
