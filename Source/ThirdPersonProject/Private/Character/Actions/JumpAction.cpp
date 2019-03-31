// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpAction.h"
#include "BaseCharacter.h"
#include "CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UJumpAction::UJumpAction()
{
    bUseAnimationEvents = true;
    Type = EActionType::Jump;
}

void UJumpAction::StartAnimationEvent()
{
    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    }
}

void UJumpAction::StopAnimationEvent()
{
    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
}
