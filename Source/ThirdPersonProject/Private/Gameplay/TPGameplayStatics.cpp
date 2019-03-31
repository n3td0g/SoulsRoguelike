// Fill out your copyright notice in the Description page of Project Settings.

#include "TPGameplayStatics.h"
#include "ThirdPersonWorldSettings.h"
#include "TargetsManager.h"

class ATargetsManager* UTPGameplayStatics::GetTargetsManager(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (!World)
	{
		return nullptr;
	}

	if (auto WorldSettings = Cast<AThirdPersonWorldSettings>(World->GetWorldSettings()))
	{
		return WorldSettings->GetTargetManager();
	}

	return nullptr;
}
