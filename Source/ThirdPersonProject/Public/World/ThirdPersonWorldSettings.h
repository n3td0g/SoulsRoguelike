// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "ThirdPersonWorldSettings.generated.h"

class ATargetsManager;

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API AThirdPersonWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	ATargetsManager* GetTargetManager();

private:
	UPROPERTY(Transient)
	ATargetsManager* TargetManager;
};
