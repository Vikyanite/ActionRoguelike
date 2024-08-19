// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *ActionName.ToString());
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *ActionName.ToString());
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
}

USActionComponent* USAction::GetOwningComponent()
{
	return Cast<USActionComponent>(GetOuter());
}

UWorld* USAction::GetWorld() const
{
	if (USActionComponent* OwnerComponent = Cast<USActionComponent>(GetOuter()))
	{
		return OwnerComponent->GetWorld();
	}
	return nullptr;
}
