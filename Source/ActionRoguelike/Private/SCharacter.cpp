// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Public/SCharacter.h"

#include "SActionComponent.h"
#include "ActionRoguelike/Public/SAttributeComponent.h"
#include "ActionRoguelike/Public/SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	
	this->AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	this->AddMovementInput(UKismetMathLibrary::GetRightVector(ControlRot), Value);
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void ASCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}

void ASCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ASCharacter::StartSprint()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::StopSprint()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < .0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRageChange(InstigatorActor, RageDelta);
	}

	if (NewHealth == 0.f)
	{
		APlayerController* PC = CastChecked<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::StopSprint);
	
}

void ASCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

