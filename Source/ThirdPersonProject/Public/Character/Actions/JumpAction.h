// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Actions/BaseMontageAction.h"
#include "JumpAction.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UJumpAction : public UBaseMontageAction
{
	GENERATED_BODY()
	
public:
    UJumpAction();

protected:
    virtual void StartAnimationEvent() override;
    virtual void StopAnimationEvent() override;
};
