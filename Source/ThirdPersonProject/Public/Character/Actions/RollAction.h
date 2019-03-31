// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Actions/BaseMontageAction.h"
#include "RollAction.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API URollAction : public UBaseMontageAction
{
	GENERATED_BODY()
	
public:
    URollAction();

    virtual bool Activate() override;

protected:
    virtual void StartAnimationEvent() override;
    virtual void StopAnimationEvent() override;
};
