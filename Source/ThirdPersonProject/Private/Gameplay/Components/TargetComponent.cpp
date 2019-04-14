// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetComponent.h"
#include "TargetsManager.h"
#include "TPGameplayStatics.h"

// Sets default values for this component's properties
UTargetComponent::UTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bIsActive = true;

	SetHiddenInGame(true);
}

// Called when the game starts
void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (auto TargetsManager = UTPGameplayStatics::GetTargetsManager(this))
	{
		TargetsManager->AddTarget(this);
	}	
}

void UTargetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (auto TargetsManager = UTPGameplayStatics::GetTargetsManager(this))
	{
		TargetsManager->RemoveTarget(this);
	}
}