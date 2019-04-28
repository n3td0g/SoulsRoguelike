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

    virtual void BeginPlay() override;

protected:
    UFUNCTION()
    void OnHealthValueChanged(float NewValue, EStatsType InStatType);

    UFUNCTION()
    void HideComponent();

protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Visibility)
    float HideComponentDelay = 7.0f;

private:
    FTimerHandle HideWidgetTimer;
	
    class AThirdPersonProjectCharacter* LocalCharacter;
};
