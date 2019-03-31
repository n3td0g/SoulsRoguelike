// Fill out your copyright notice in the Description page of Project Settings.

#include "RollAction.h"
#include "BaseCharacter.h"
#include "CharacterAnimInstance.h"


URollAction::URollAction()
{
    bUseAnimationEvents = true;
    Type = EActionType::Roll;
}

bool URollAction::Activate()
{
    if (Super::Activate())
    {
        if (OwnerCharacter)
        {
            const FVector& InputVector = OwnerCharacter->Internal_GetLastMovementInputVector();
            OwnerCharacter->SetActorRotation(InputVector.Rotation());
        }
        return true;
    }
    return false;
}

void URollAction::StartAnimationEvent()
{

}

void URollAction::StopAnimationEvent()
{

}
