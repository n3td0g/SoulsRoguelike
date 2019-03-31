// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StartActionEvent.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Start Action Event"))
class THIRDPERSONPROJECT_API UAnimNotify_StartActionEvent : public UAnimNotify
{
	GENERATED_BODY()
	
public:
    UAnimNotify_StartActionEvent();

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
