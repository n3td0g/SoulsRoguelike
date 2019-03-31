#include "CharacterActionsComponent.h"
#include "BaseAction.h"

UCharacterActionsComponent::UCharacterActionsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCharacterActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int32 I = ActionsToTick.Num() - 1; I >= 0; --I)
    {
        ActionsToTick[I]->ActionTick(DeltaTime);
    }

    if (!CurrentAction)
    {
        if (NextAction != EActionType::None)
        {
            EActionType NewActionType = NextAction;
            NextAction = EActionType::None;
            if (GetWorld()->TimeSeconds - NextActionActivationTime < NextActionMemoryTime)
            {
                StartAction(NewActionType);
            }
        }
    }
}

bool UCharacterActionsComponent::StartAction(EActionType NewActionType)
{
	auto NewActionPtr = CharacterActions.Find(NewActionType);
	if (!NewActionPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action type <%s> not found."), *UBaseAction::GetActionTypeAsString(NewActionType));
		return false;
	}

	auto NewAction = *NewActionPtr;
	if (!IsValid(NewAction))
	{
		UE_LOG(LogTemp, Warning, TEXT("Action <%s> is not valid."), *UBaseAction::GetActionTypeAsString(NewActionType));
		return false;
	}

	if (IsValid(CurrentAction))
	{
		if (!NewAction->HasHigherPriority(CurrentAction->GetActionType()))
		{
            NextAction = NewActionType;
            NextActionActivationTime = GetWorld()->TimeSeconds;
			return false;
		}

        if (!CurrentAction->StopAction(true))
        {
            return false;
        }
	}

	if (NewAction->Activate())
	{
		CurrentAction = NewAction;
		CurrentAction->OnStopAction.AddDynamic(this, &UCharacterActionsComponent::OnStopAction);
        if (CurrentAction->GetTickOnlyIfActive())
        {
            ActionsToTick.Push(CurrentAction);
        }
        //If action was stopped in function Activate()
        if (!CurrentAction->GetIsActive())
        {
            StopCurrentAction();
            return false;
        }
	}
	else
	{
		CurrentAction = nullptr;
	}

	return true;
}

bool UCharacterActionsComponent::StopAction(EActionType ActionType)
{
	if (IsValid(CurrentAction))
	{
		if (CurrentAction->GetActionType() == ActionType)
		{
			return StopCurrentAction();
		}
	}
	return false;
}

UBaseAction* UCharacterActionsComponent::GetPrevAction()
{
    if (PrevAction)
    {
        if (GetWorld()->TimeSeconds - PrevActionTime > NextActionMemoryTime)
        {
            PrevAction = nullptr;
        }
    }
    return PrevAction;
}

bool UCharacterActionsComponent::StopCurrentAction()
{
	if (IsValid(CurrentAction))
	{
        OnBeforeStopAction.Broadcast(CurrentAction);

        if (CurrentAction->GetActionType() == NextAction)
        {
            NextAction = EActionType::None;
            if (CurrentAction->ContinueAction())
            {
                return true;
            }
        }

		return CurrentAction->StopAction(false);
	}
	return false;
}

void UCharacterActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateActions();
}

void UCharacterActionsComponent::OnStopAction(UBaseAction* Action)
{
	if (Action == CurrentAction)
	{
		if (IsValid(CurrentAction))
		{
            OnActionStopped.Broadcast(CurrentAction);
			CurrentAction->OnStopAction.RemoveDynamic(this, &UCharacterActionsComponent::OnStopAction);
            if (CurrentAction->GetTickOnlyIfActive())
            {
                ActionsToTick.Remove(CurrentAction);
            }

            PrevAction = CurrentAction;
            PrevActionTime = GetWorld()->TimeSeconds;
		}

		CurrentAction = nullptr;
	}
}

void UCharacterActionsComponent::CreateActions()
{
	for (auto Action : Actions)
	{
		if (Action)
        {
            if (CharacterActions.Contains(Action->GetActionType()))
            {
                UE_LOG(LogTemp, Warning, TEXT("Action type <%s> is already exist."), *UBaseAction::GetActionTypeAsString(Action->GetActionType()));
                continue;
            }

            if (Action->GetActionType() == EActionType::None)
            {
                UE_LOG(LogTemp, Warning, TEXT("Action type in None."));
                continue;
            }

            Action->SetOwner(GetOwner());
            Action->Init();
            CharacterActions.Add(Action->GetActionType(), Action);
            if (!Action->GetTickOnlyIfActive())
            {
                ActionsToTick.Push(Action);
            }
		}
	}
}

