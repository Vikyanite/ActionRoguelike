// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	AttackAnimDelay = 0.2f;
	HandSocketName = "Muzzle_01";
	TimeToHitParamName = "TimeToHit";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (ASCharacter* Character = Cast<ASCharacter>(Instigator))
	{
		Character->PlayAnimMontage(AttackAnim);

		// casting spell
		UGameplayStatics::SpawnEmitterAttached(CastSpellVFX, Character->GetMesh(), HandSocketName, FVector::ZeroVector,
		                                       FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandler_PrimaryAttack;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, "AttackDelay_TimeElapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandler_PrimaryAttack, TimerDel, AttackAnimDelay, false);
	}
}

void USAction_ProjectileAttack::AttackDelay_TimeElapsed(ACharacter* Character)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = Character->GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Character;

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Character);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FVector TraceStart = Character->GetPawnViewLocation();

		FVector TraceEnd = TraceStart + (Character->GetControlRotation().Vector() * 5000.f);
		
		FHitResult Hit;
		
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
		
	}
	
	StopAction(Character);
}

