// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Actions/BaseMontageAction.h"
#include "DeathAction.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UDeathAction : public UBaseMontageAction
{
	GENERATED_BODY()
	
public:
    UDeathAction();
	
    virtual bool Activate() override;
    virtual bool StopAction(bool bIsForce) override;

protected:
    virtual bool CanBeStopped() override;
};
