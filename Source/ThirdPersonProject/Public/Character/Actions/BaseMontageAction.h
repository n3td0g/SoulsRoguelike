// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Actions/BaseAction.h"
#include "BaseMontageAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDPERSONPROJECT_API UBaseMontageAction : public UBaseAction
{
	GENERATED_BODY()
	
public:
    virtual void Init() override;

    virtual bool Activate() override;

    virtual bool StopAction(bool bIsForce) override;

protected:
    virtual void StartAnimationEvent() { }
    virtual void StopAnimationEvent() { }

    UFUNCTION()
    void StartAnimationEventBinding();

    UFUNCTION()
    void StopAnimationEventBinding();

    bool TryToSpendStamind();

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Montage)
    class UAnimMontage* CurrentMontage;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Montage)
    FName SectionName = NAME_None;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Montage)
    float RequiredStamina = -100.0f;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Montage)
    bool bUseAnimationEvents = false;

    UPROPERTY(Transient)
    class UCharacterAnimInstance* CharacterAnimInstance;    
};
