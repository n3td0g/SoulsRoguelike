// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatsType.h"
#include "Components/ProgressBar.h"
#include "StatProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UStatProgressBar : public UProgressBar
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void Initialize(class ABaseCharacter* Character);

    UFUNCTION(BlueprintCallable)
    void Update(float DeltaTime);
	
public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Stats)
    float InterpolationSpeed = 0.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Stats)
    EStatsType StatType = EStatsType::Health;
private:
    UFUNCTION()
    void OnStatValueChanged(float NewValue, EStatsType InStatType);

    UFUNCTION()
    void OnStatMaxValueChanged(float NewMaxValue, EStatsType InStatType);

    void UpdateProgress(float NewCurrentValue);
private:
    float CurrentValue = 0.0f;
    float TargetPercent = 1.0f;
    float MaxValue = 0.0f;
};
