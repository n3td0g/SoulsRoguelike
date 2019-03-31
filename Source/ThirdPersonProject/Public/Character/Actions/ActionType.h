#pragma once

#include "ActionType.generated.h"

class UBaseAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopActionSignature, UBaseAction*, Action);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeforeStopActionSignature, UBaseAction*, Action);

UENUM(BlueprintType, meta = (Bitflags))
enum class EActionType : uint8
{
    None,
    Run,
    Jump,
    Backstep,
    Roll,
    Attack,
    Kick,
    UseSelectedItem,
    UseSpell,
    Backstab,
    CriticalDamage,
    Block,
    Stagger,
    StunLock,
    Interact,
    Death
};