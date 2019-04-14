// Fill out your copyright notice in the Description page of Project Settings.

#include "StartActionAsync.h"
#include "BaseCharacter.h"
#include "CharacterActionsComponent.h"


UStartActionAsync* UStartActionAsync::StartActionAsync(UObject* WorldContextObject, class ABaseCharacter* InCharacter, EActionType InActionType)
{
    UStartActionAsync* Action = NewObject<UStartActionAsync>();
    Action->Character = InCharacter;
    Action->ActionType = InActionType;
    return Action;
}

void UStartActionAsync::Activate()
{
    if (!IsValid(Character))
    {
        OnFailed.Broadcast();
        return;
    }

    auto ActionComponent = Character->GetActionComponent();

    if (!IsValid(ActionComponent))
    {
        OnFailed.Broadcast();
        return;
    }

    if (ActionComponent->StartAction(ActionType))
    {
        ActionComponent->OnActionStopped.AddUniqueDynamic(this, &UStartActionAsync::OnActionStopped);
        ActionComponent->OnBeforeStopAction.AddUniqueDynamic(this, &UStartActionAsync::OnBeforeStopAction);
    }
	else
	{
		OnFailed.Broadcast();
	}
}

void UStartActionAsync::OnBeforeStopAction(UBaseAction* Action)
{
    OnBeforeStop.Broadcast();
}

void UStartActionAsync::OnActionStopped(UBaseAction* Action)
{
    auto ActionComponent = Character->GetActionComponent();    
    ActionComponent->OnActionStopped.RemoveAll(this);
    ActionComponent->OnBeforeStopAction.RemoveAll(this);
    OnSuccess.Broadcast();
}

