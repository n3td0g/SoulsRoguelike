// Fill out your copyright notice in the Description page of Project Settings.

#include "RunAction.h"
#include "BaseCharacter.h"
#include "CharacterActionsComponent.h"
#include "CharacterStatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

URunAction::URunAction()
{
    bTickOnlyIfActive = true;
    Type = EActionType::Run;
}

void URunAction::Init()
{
	Super::Init();

	if (IsValid(OwnerCharacter))
	{
        DefaultWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
	}
}

void URunAction::ActionTick(float DeltaTime)
{
	Super::ActionTick(DeltaTime);

	if (!IsValid(OwnerCharacter))
	{
		return;
	}

    if (OwnerCharacter->GetCharacterMovement())
    {
        if (OwnerCharacter->GetCharacterMovement()->IsFalling())
        {
            bIsRunning = false;
            return;
        }
    }    
   /* const FVector& LastControlInputVector = OwnerCharacter->GetLastMovementInputVector();
    const FVector& ControllerInputNormal = LastControlInputVector.GetSafeNormal();*/
    float MovementSpeed = FVector::DotProduct(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorForwardVector());
    bIsRunning = MovementSpeed > MinRunSpeed;

    if (bIsRunning)
    {
        if (!OwnerCharacter->GetStatsComponent()->TryToChangeStatValue(EStatsType::Stamina, -StaminaPerSecond * DeltaTime))
        {
            StopAction(true);
        }
    }
}

bool URunAction::Activate()
{
	if (!IsValid(OwnerCharacter))
	{
		return false;
	}

	if (OwnerCharacter->GetStatsComponent()->GetStatData(EStatsType::Stamina).Value > StaminaToStartRun)
	{   
        if (Super::Activate())
        {
            OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
            return true;
        }		
	}

	return false;
}

bool URunAction::StopAction(bool bIsForce)
{
	if (Super::StopAction(bIsForce))
	{
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
		return true;
	}

	return false;
}
