// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonWorldSettings.h"
#include "EngineUtils.h"
#include "TargetsManager.h"

ATargetsManager* AThirdPersonWorldSettings::GetTargetManager()
{
	if (!IsValid(TargetManager))
	{
		for (TActorIterator<ATargetsManager> Manager(GetWorld()); Manager; ++Manager)
		{
			TargetManager = *Manager;
			break;
		}
	}
	return TargetManager;
}