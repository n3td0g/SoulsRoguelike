// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetIndicatorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"	
#include "LookToTargetComponent.h"
#include "TargetComponent.h"

UTargetIndicatorComponent::UTargetIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UTargetIndicatorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(Camera))
	{
		if (IsValid(Target))
		{
			const FVector& TargetLocation = Target->GetComponentLocation();
			const FVector& CameraLocation = Camera->GetComponentLocation();

			SetWorldLocation(TargetLocation);
		}
	}
}

void UTargetIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHiddenInGame(true, true);

	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (!IsValid(Camera))
	{
		return;
	}

	if (auto TargetComponent = GetOwner()->FindComponentByClass<ULookToTargetComponent>())
	{
		TargetComponent->OnTargetChanged.AddDynamic(this, &UTargetIndicatorComponent::TargetChanged);
	}
}

void UTargetIndicatorComponent::TargetChanged(class UTargetComponent* NewTarget)
{
	if (IsValid(NewTarget))
	{
		bAbsoluteLocation = true;
		SetComponentTickEnabled(true);
		SetHiddenInGame(false, true);
	}
	else
	{
		bAbsoluteLocation = false;
		
		SetComponentTickEnabled(false);
		SetHiddenInGame(true, true);

		SetRelativeLocation(FVector::ZeroVector);
	}

	Target = NewTarget;
}
