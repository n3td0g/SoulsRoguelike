// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseMontageAction.h"
#include "BaseCharacter.h"
#include "CharacterAnimInstance.h"
#include "CharacterStatsComponent.h"

void UBaseMontageAction::Init()
{
    Super::Init();

    if (IsValid(OwnerCharacter) && bUseAnimationEvents)
    {
        CharacterAnimInstance = Cast<UCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
    }
}

bool UBaseMontageAction::Activate()
{
    if (!CurrentMontage)
    {
        return false;
    }

    if (IsValid(OwnerCharacter))
    {
        if (!TryToSpendStamind())
        {
            return false;
        }

        if (Super::Activate())
        {
            if (IsValid(CharacterAnimInstance))
            {
                CharacterAnimInstance->OnStartActionEvent.AddDynamic(this, &UBaseMontageAction::StartAnimationEventBinding);
                CharacterAnimInstance->OnStopActionEvent.AddDynamic(this, &UBaseMontageAction::StopAnimationEventBinding);
            }
            
            OwnerCharacter->PlayAnimMontage(CurrentMontage, 1.0f, SectionName);
            return true;
        }
    }

    return false;
}

bool UBaseMontageAction::StopAction(bool bIsForce)
{
    if (Super::StopAction(bIsForce))
    {
        if (IsValid(CharacterAnimInstance))
        {
            CharacterAnimInstance->OnStartActionEvent.RemoveDynamic(this, &UBaseMontageAction::StartAnimationEventBinding);
            CharacterAnimInstance->OnStopActionEvent.RemoveDynamic(this, &UBaseMontageAction::StopAnimationEventBinding);
        }

        if (bUseAnimationEvents)
        {
            StopAnimationEvent();
        }

        if (IsValid(OwnerCharacter))
        {
            OwnerCharacter->StopAnimMontage(CurrentMontage);
        }

        return true;
    }
    return false;
}

void UBaseMontageAction::StartAnimationEventBinding()
{
    StartAnimationEvent();
}

void UBaseMontageAction::StopAnimationEventBinding()
{
    StopAnimationEvent();
}

bool UBaseMontageAction::TryToSpendStamind()
{
    if (RequiredStamina > 0.0f)
    {
        return OwnerCharacter->GetStatsComponent()->TryToChangeStatValue(EStatsType::Stamina, -RequiredStamina);
    }

    return true;
}
