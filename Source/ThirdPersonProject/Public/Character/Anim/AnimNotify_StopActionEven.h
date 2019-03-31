// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StopActionEven.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Stop Action Even"))
class THIRDPERSONPROJECT_API UAnimNotify_StopActionEven : public UAnimNotify
{
	GENERATED_BODY()
	
public:
    UAnimNotify_StopActionEven();

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
