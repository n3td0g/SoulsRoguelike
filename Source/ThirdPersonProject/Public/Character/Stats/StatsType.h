#pragma once

#include "StatsType.generated.h"

UENUM(BlueprintType)
enum class EStatsType : uint8
{
    Health,
    Stamina,
    Poise,
    Strength,
    PhysicalDefence,
    Power,
    Mana,
    Count	UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FStatData
{
    GENERATED_BODY()

    public:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    EStatsType Type;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float Value = 100.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float MinValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float CriticalValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float MaxValue = 100.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float DelayBeforeRecovery = 0.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float RecoverySpeed = 0.0f;

    UPROPERTY(Transient)
    float RecoveryStartTime;
};