// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapon/BaseWeapon.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
    virtual void StartAttack(EAttackType AttackType) override;
    virtual void StopAttack() override;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void Trace();

protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Debug)
    bool bDebug = false;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Trace)
    FName ComponentToTraceTag = TEXT("ComponentToTrace");

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Trace)
    float TraceUpdateTime = 0.05f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Trace)
    float TraceRadius = 5.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<USceneComponent*> ComponentsToTrace;
private:
    void UpdatePrevTracePoints();

private:
    FTimerHandle TraceTimer;

    TArray<FVector> PrevTracePoint;
    TArray<AActor*> ActorsToIgnore;
};
