// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackAction.h"
#include "TraceChannels.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "CharacterActionsComponent.h"
#include "CharacterAnimInstance.h"
#include "BaseCharacter.h"
#include "RunAction.h"

UAttackAction::UAttackAction()
{
    bUseAnimationEvents = true;
    Type = EActionType::Attack;

	ChannelsToTrace.Add(UEngineTypes::ConvertToTraceType(ECC_PlayerDamage));
	ChannelsToTrace.Add(UEngineTypes::ConvertToTraceType(ECC_EnemyDamage));
}

void UAttackAction::Init()
{
	Super::Init();

	//Finding trace channel for character (ignore self trace channel)
	if (OwnerCharacter->GetMesh())
	{
		for (int32 I = ChannelsToTrace.Num() - 1; I >= 0; --I)
		{
			if (OwnerCharacter->GetMesh()->GetCollisionResponseToChannel(UEngineTypes::ConvertToCollisionChannel(ChannelsToTrace[I])) != ECR_Ignore)
			{
				ChannelsToTrace.RemoveAt(I);
			}
		}

		if (ChannelsToTrace.Num())
		{
			ChannelToTrace = ChannelsToTrace[0];
		}
	}
}

bool UAttackAction::ContinueAction()
{
    if (!CharacterAnimInstance)
    {
        return false;
    }

    EAttackType AttackType = GetAttackType();
    if (AttackType != CurrAttackType)
    {
        return false;
    }

    int32 NumOfCombo = 1;
    CurrentMontage = CurrentWeapon->GetAttackMontage(AttackType, NumOfCombo, RequiredStamina);
    if (NumOfCombo <= 1)
    {
        return false;
    }
    if (!CurrentMontage)
    {
        return false;
    }

    ComboIndex = (ComboIndex + 1) % NumOfCombo;
    if (ComboIndex == 0)
    {
        return false;
    }

    if (!TryToSpendStamind())
    {
        return false;
    }

    FName NewSection = *FString::Printf(TEXT("%s%d"), *ComboNamePrefix, ComboIndex);
    FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboNamePrefix, ComboIndex + 1);

    CharacterAnimInstance->Montage_SetNextSection(NewSection, NextSection, CurrentMontage);
    
    return true;
}

bool UAttackAction::Activate()
{
    CurrentWeapon = OwnerCharacter->GetCurrentWeapon();
	CurrentWeapon->ChannelToTrace = ChannelToTrace;
    if (IsValid(CurrentWeapon))
    {
        EAttackType AttackType = GetAttackType();
        int32 NumOfCombo = 1;
        CurrentMontage = CurrentWeapon->GetAttackMontage(AttackType, NumOfCombo, RequiredStamina);
        CurrAttackType = AttackType;
        return Super::Activate();
    }
    return false;
}

bool UAttackAction::StopAction(bool bIsForce)
{
    if (Super::StopAction(bIsForce))
    {
        SectionName = NAME_None;        
        ComboIndex = 0;
        return true;
    }
    return false;
}

void UAttackAction::StartAnimationEvent()
{
    Super::StartAnimationEvent();

    CurrentWeapon->StartAttack(CurrAttackType);
}

void UAttackAction::StopAnimationEvent()
{
    Super::StopAnimationEvent();

    CurrentWeapon->StopAttack();
}

EAttackType UAttackAction::GetAttackType() const
{
    if (IsOnRunAttack())
    {
        return EAttackType::AfterRun;
    }
    return EAttackType::Default;
}

bool UAttackAction::IsOnRunAttack() const
{
    if (auto PrevAction = OwnerCharacter->GetActionComponent()->GetPrevAction())
    {
        if (PrevAction->GetActionType() == EActionType::Run)
        {
            if (auto RunAction = Cast<URunAction>(PrevAction))
            {
                if (RunAction->GetIsRunnning())
                {
                    return true;
                }
            }
        }
    }
    return false;
}