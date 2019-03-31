// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStatsComponent.h"

// Sets default values for this component's properties
UCharacterStatsComponent::UCharacterStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    bIsActive = true;

	for (uint8 I = 0; I < (uint8)EStatsType::Count; ++I)
	{
		FStatData Data;
		Data.Type = (EStatsType)I;
		Stats.Push(Data);
	}
}

// Called every frame
void UCharacterStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!bIsActive)
    {
        return;
    }

	float WorldTime = GetWorld()->GetTimeSeconds();
	for (int32 I = StatsToRecovery.Num() - 1; I >= 0 ; --I)
	{
		if (FStatData* Data = StatsToRecovery[I])
		{
			if (Data->RecoveryStartTime < WorldTime)
			{
                ChangeStatDataValue(*Data, Data->RecoverySpeed * DeltaTime);
			}			
		}
		else
		{
			StatsToRecovery.RemoveAt(I);
		}
	}
}

void UCharacterStatsComponent::ChangeStatMaxValue(EStatsType Type, float NewMaxValue)
{
	int32 Index = (int32)Type;
	check(Stats.IsValidIndex(Index));

	FStatData& Data = Stats[Index];
	Data.MaxValue = Index;
    AddToRecoveryList(Data);

    OnMaxValueChanged.Broadcast(Data.MaxValue, Data.Type);
}

void UCharacterStatsComponent::AddToRecoveryList(FStatData& Data)
{
    if (Data.RecoverySpeed > 0.0f)
    {
        StatsToRecovery.AddUnique(&Data);
        Data.RecoveryStartTime = GetWorld()->GetTimeSeconds() + Data.DelayBeforeRecovery;
    }
}

float UCharacterStatsComponent::ChangeStatValue(EStatsType Type, float Delta)
{
	FStatData& Data = GetStatData(Type);
    ChangeStatValue(Data, Delta);
    return Data.Value;
}

void UCharacterStatsComponent::ChangeStatValue(FStatData& Data, float Delta)
{
    ChangeStatDataValue(Data, Delta);
    if (Data.Value < Data.MaxValue)
    {
        AddToRecoveryList(Data);
    }
}

void UCharacterStatsComponent::ChangeStatDataValue(FStatData& Data, float Delta)
{
    if (!bIsActive)
    {
        return;
    }

	Data.Value = FMath::Clamp(Data.Value + Delta, Data.MinValue, Data.MaxValue);
    OnValueChanged.Broadcast(Data.Value, Data.Type);

	if (Data.RecoverySpeed > 0.0f)
	{
		if (Data.Value >= Data.MaxValue)
		{
			Data.Value = Data.MaxValue;
			StatsToRecovery.Remove(&Data);
		}
	}
}

bool UCharacterStatsComponent::TryToChangeStatValue(EStatsType Type, float Delta)
{
    FStatData& Data = GetStatData(Type);
    if (Data.Value > Data.CriticalValue)
    {
        ChangeStatValue(Data, Delta);
        return true;
    }
    return false;
}

FStatData& UCharacterStatsComponent::GetStatData(EStatsType Type)
{
	int32 Index = (int32)Type;

	check(Stats.IsValidIndex(Index));

	return Stats[Index];
}

float UCharacterStatsComponent::GetDataValue(EStatsType Type)
{
	FStatData& Data = GetStatData(Type);
	return Data.Value;
}

// Called when the game starts
void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Data : Stats)
	{
		if (Data.Value < Data.MaxValue && Data.RecoverySpeed > 0)
		{
			AddToRecoveryList(Data);
		}
	}
}