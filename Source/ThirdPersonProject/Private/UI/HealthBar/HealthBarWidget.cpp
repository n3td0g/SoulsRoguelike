// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBarWidget.h"
#include "BaseCharacter.h"
#include "StatProgressBar.h"

void UHealthBarWidget::SetCharacter(class ABaseCharacter* Character)
{
    if (IsValid(HealthBar))
    {
        HealthBar->Initialize(Character);
    }
}

void UHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (IsValid(HealthBar))
    {
        HealthBar->Update(InDeltaTime);
    }
}
