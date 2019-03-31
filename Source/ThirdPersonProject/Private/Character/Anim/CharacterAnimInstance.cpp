// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"
#include "BaseCharacter.h"
#include "CharacterActionsComponent.h"

void UCharacterAnimInstance::NativeBeginPlay()
{
    OwnerCharacter = Cast<ABaseCharacter>(GetOwningActor());
}

void UCharacterAnimInstance::StopCurrentAction()
{
    if (OwnerCharacter)
    {
        OwnerCharacter->GetActionComponent()->StopCurrentAction();
    }
}

void UCharacterAnimInstance::StartActionEvent()
{
    OnStartActionEvent.Broadcast();
}

void UCharacterAnimInstance::StopActionEvent()
{
    OnStopActionEvent.Broadcast();
}
