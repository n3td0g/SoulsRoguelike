// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StopAction.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Stop Action"))
class THIRDPERSONPROJECT_API UAnimNotify_StopAction : public UAnimNotify
{
	GENERATED_BODY()
	
public:
    UAnimNotify_StopAction();

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
