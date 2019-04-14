// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharacterActionsComponent.h"
#include "BaseWeapon.h"
#include "RunAction.h"

AThirdPersonProjectCharacter::AThirdPersonProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

    ActionComponent->OnActionStopped.AddDynamic(this, &AThirdPersonProjectCharacter::OnActionStopped);
}

void AThirdPersonProjectCharacter::SwitchWeapon(float Sign)
{
    int32 Delta = Sign > 0.0f ? 1 : -1;
    int32 WeaponIndex = CurrentWeaponIndex + Delta;
    if (WeaponIndex < 0)
    {
        WeaponIndex = PlayerWeaponClasses.Num() - 1;
    }
    else
    {
        WeaponIndex = WeaponIndex % PlayerWeaponClasses.Num();
    }
    
    TryToSwitchWeapon(WeaponIndex);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdPersonProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThirdPersonProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdPersonProjectCharacter::LookUpAtRate);

    PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AThirdPersonProjectCharacter::RunButtonPressed);
    PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AThirdPersonProjectCharacter::RunButtonReleased);
}

void AThirdPersonProjectCharacter::OnActionStopped(UBaseAction* CurrentAction)
{
    if(!bIsRunButtonPressed)
    {
        return;
    }
    if (!IsValid(CurrentAction))
    {
        return;
    }

    if (CurrentAction->GetActionType() != EActionType::Run)
    {
        ActionComponent->StartAction(EActionType::Run);
    }
}

void AThirdPersonProjectCharacter::TryToSwitchWeapon(int32 NewWeaponIndex)
{
    if (CurrentWeaponIndex == NewWeaponIndex)
    {
        return;
    }
    if (PlayerWeaponClasses.IsValidIndex(NewWeaponIndex))
    {
        if (auto CurrentAction = ActionComponent->GetCurrentAction())
        {
            if (CurrentAction->GetActionType() == EActionType::Attack)
            {
                return;
            }
        }

        if (IsValid(TestWeapon))
        {
            TestWeapon->Destroy();
            TestWeapon = nullptr;
        }

        TestWeaponClass = PlayerWeaponClasses[NewWeaponIndex];
        CurrentWeaponIndex = NewWeaponIndex;
        GetCurrentWeapon();
    }
}

void AThirdPersonProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonProjectCharacter::RunButtonPressed()
{
    const float Epsilon = 0.00001f;
    bIsRunButtonPressed = true;
    if (LastControlInputVector.SizeSquared() < Epsilon)
    {
        ActionComponent->StartAction(EActionType::Backstep);
    }
    else
    {
        if (auto RunAction = Cast<URunAction>(ActionComponent->GetPrevAction()))
        {
            if (RunAction->GetIsRunnning())
            {
                ActionComponent->StartAction(EActionType::Jump);
                return;
            }
        }
        ActionComponent->StartAction(EActionType::Run, false);
        LastRunInputTime = GetWorld()->TimeSeconds;
    }
}

void AThirdPersonProjectCharacter::RunButtonReleased()
{
    bIsRunButtonPressed = false;
    if (ActionComponent->StopAction(EActionType::Run))
    {
        if (GetWorld()->TimeSeconds - LastRunInputTime < TimeToRoll)
        {
            ActionComponent->StartAction(EActionType::Roll);
        }
    }
}

void AThirdPersonProjectCharacter::BeginPlay()
{
    Super::BeginPlay();

    TryToSwitchWeapon(0);
}