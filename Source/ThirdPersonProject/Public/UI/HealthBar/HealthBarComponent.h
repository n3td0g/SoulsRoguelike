// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "StatsType.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
    UHealthBarComponent();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    virtual void BeginPlay() override;

protected:
    UFUNCTION()
    void OnHealthValueChanged(float NewValue, EStatsType InStatType);

    UFUNCTION()
    void HideComponent();

protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Visibility)
    float HideComponentDelay = 7.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Scale)
    float MinScale = 1.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Scale)
    float MaxScale = 10.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Scale)
    float MinScaleDistance = 100.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Scale)
    float MaxScaleDistance = 5000.0f;

private:
    FTimerHandle HideWidgetTimer;
	
    class AThirdPersonProjectCharacter* LocalCharacter;
};
