// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimationAttackEventSignature);

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void NativeBeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void StopCurrentAction();

    UFUNCTION(BlueprintCallable)
    void StartActionEvent();

    UFUNCTION(BlueprintCallable)
    void StopActionEvent();

public:
    UPROPERTY(BlueprintAssignable)
    FAnimationAttackEventSignature OnStartActionEvent;

    UPROPERTY(BlueprintAssignable)
    FAnimationAttackEventSignature OnStopActionEvent;

protected:
    UPROPERTY(BlueprintReadOnly)
    class ABaseCharacter* OwnerCharacter;
};
