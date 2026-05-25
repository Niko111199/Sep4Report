// Copyright Epic Games, Inc. All Rights Reserved.

#include "Sep4Character.h"
#include "Sep4Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASep4Character

ASep4Character::ASep4Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

//////////////////////////////////////////////////////////////////////////// Input

void ASep4Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASep4Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASep4Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASep4Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASep4Character::Server_TeleportPlayer_Implementation(APlayerController* TargetPlayer)
{
	if (!TargetPlayer)
	{
		return;
	}

	APawn* TargetPawn = TargetPlayer->GetPawn();

	if(!TargetPawn)
	{
		return;
	}

	FVector TeleportLocation = FVector(0.0f, 0.0f, 0.0f);

	TargetPawn->TeleportTo(TeleportLocation, TargetPawn->GetActorRotation());
}

void ASep4Character::Server_TeleportPlayerTo_Implementation(APlayerController* TargetPlayer, FVector Position)
{
	if (!TargetPlayer)
		return;

	APawn* TargetPawn = TargetPlayer->GetPawn();

	if (!TargetPawn)
		return;

	TargetPawn->TeleportTo(Position, TargetPawn->GetActorRotation());
}

void ASep4Character::TeleportPlayer()
{
	UWorld* world = GetWorld();

	if (!world) return;

	APlayerController* TargetPlayer = nullptr;

	for(FConstPlayerControllerIterator Iterator = world->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* playercontroller = Iterator->Get();

		if(playercontroller && playercontroller !=GetController())
		{
			TargetPlayer = playercontroller;
			break;
		}
	}

	if (TargetPlayer)
	{
		Server_TeleportPlayer(TargetPlayer);
	}
}

void ASep4Character::TeleportPlayerTo(APlayerController* TargetPlayer, FVector Position)
{
	Server_TeleportPlayerTo(TargetPlayer, Position);
}


void ASep4Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASep4Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}