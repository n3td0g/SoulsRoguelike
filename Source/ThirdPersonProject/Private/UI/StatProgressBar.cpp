// Fill out your copyright notice in the Description page of Project Settings.

#include "StatProgressBar.h"
#include "BaseCharacter.h"
#include "CharacterStatsComponent.h"

void UStatProgressBar::Initialize(ABaseCharacter* Character)
{
    if (IsValid(Character))
    {
        Character->GetStatsComponent()->OnValueChanged.AddDynamic(this, &UStatProgressBar::OnStatValueChanged);
        Character->GetStatsComponent()->OnMaxValueChanged.AddDynamic(this, &UStatProgressBar::OnStatMaxValueChanged);

        const FStatData& StatData = Character->GetStatsComponent()->GetStatData(StatType);

        CurrentValue = StatData.Value;
        OnStatMaxValueChanged(StatData.MaxValue, StatType);
    }
}

void UStatProgressBar::Update(float DeltaTime)
{
    if (InterpolationSpeed > 0.0f)
    {
        float NewPercent = FMath::FInterpConstantTo(Percent, TargetPercent, DeltaTime, InterpolationSpeed);
        SetPercent(NewPercent);
    }
}

void UStatProgressBar::OnStatValueChanged(float NewValue, EStatsType InStatType)
{
    if (InStatType != this->StatType)
    {
        return;
    }
    CurrentValue = NewValue;
    TargetPercent = NewValue / MaxValue;
    if (InterpolationSpeed == 0.0f)
    {
        SetPercent(TargetPercent);
    }
}

void UStatProgressBar::OnStatMaxValueChanged(float NewMaxValue, EStatsType InStatType)
{
    if (InStatType != this->StatType)
    {
        return;
    }
    MaxValue = NewMaxValue;
    OnStatValueChanged(CurrentValue, InStatType);
}