// Fill out your copyright notice in the Description page of Project Settings.

#include "LookToTargetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TargetsManager.h"
#include "TPGameplayStatics.h"
#include "TargetComponent.h"

// Sets default values for this component's properties
ULookToTargetComponent::ULookToTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}

// Called every frame
void ULookToTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!OwnerPawn || !OwnerPawn->GetController())
	{
		return;
	}

	if (IsValid(TargetComponent))
	{
		FVector ComponentLocation = GetComponentLocation();
		const FVector& TargetLocation = TargetComponent->GetComponentLocation();
		float DistanceToTarget = (ComponentLocation - TargetLocation).Size();
		if (DistanceToTarget > MaxDistanceToTarget)
		{
			SetTarget(nullptr);
		}
		float Offset = FMath::GetMappedRangeValueClamped(OffsetDistance, OffsetValue, DistanceToTarget);
		ComponentLocation.Z += Offset;
		const FRotator& TargetRotation = UKismetMathLibrary::FindLookAtRotation(ComponentLocation, TargetLocation);
		
		OwnerPawn->GetController()->SetControlRotation(TargetRotation);

		if (IsValid(IndicatorComponent))
		{
			IndicatorComponent->SetWorldLocation(TargetComponent->GetComponentLocation());
		}
	}
	else
	{
		SetTarget(nullptr);		
	}
}

void ULookToTargetComponent::SetTarget(UTargetComponent* NewTarget)
{
	if (NewTarget == TargetComponent)
	{
		return;
	}
	TargetComponent = NewTarget;
	bool bTargetIsValid = IsValid(TargetComponent);
	SetComponentTickEnabled(bTargetIsValid);
	if (auto MovementComponent = GetOwnerMovementComponent())
	{
		MovementComponent->bOrientRotationToMovement = !bTargetIsValid;
	}

	if (IsValid(IndicatorComponent))
	{
		IndicatorComponent->SetHiddenInGame(!bTargetIsValid);
	}

	if (bTargetIsValid)
	{
		GetWorld()->GetTimerManager().SetTimer(ChechTargetTimer, this, &ULookToTargetComponent::CheckTarget, CheckTargetRate, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ChechTargetTimer);
	}

	OnTargetChanged.Broadcast(NewTarget);
}

void ULookToTargetComponent::SelectNearestTarget(float MaxCos, float MaxDistance)
{
	if (!IsValid(OwnerPawn))
	{
		return;
	}

	if (auto Camera = OwnerPawn->FindComponentByClass<UCameraComponent>())
	{
		if (auto TargetsManager = UTPGameplayStatics::GetTargetsManager(this))
		{
			SetTarget(TargetsManager->GetNearestTarget(Camera->GetComponentLocation(), FVector::ZeroVector, Camera->GetForwardVector(), ProbeChannel, nullptr, MaxCos, MaxDistance));
		}
	}
}

void ULookToTargetComponent::SelectNextTarget(float Sign, float MaxCos /*= 0.5f*/, float MaxDistance /*= 1500.0f*/)
{
	if (!IsValid(OwnerPawn))
	{
		return;
	}

	if (!IsValid(TargetComponent))
	{
		return;
	}

	if (auto Camera = OwnerPawn->FindComponentByClass<UCameraComponent>())
	{
		if (auto TargetsManager = UTPGameplayStatics::GetTargetsManager(this))
		{
			auto NextTarget = TargetsManager->GetNearestTarget(Camera->GetComponentLocation(), Camera->GetRightVector() * Sign, 
				Camera->GetForwardVector(), ProbeChannel, TargetComponent, MaxCos, MaxDistance);
			if (IsValid(NextTarget))
			{
				SetTarget(NextTarget);
			}
		}
	}
}

// Called when the game starts
void ULookToTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());
}

UCharacterMovementComponent* ULookToTargetComponent::GetOwnerMovementComponent()
{
	return GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
}

void ULookToTargetComponent::CheckTarget()
{
	if (IsValid(TargetComponent))
	{
		if (auto Camera = OwnerPawn->FindComponentByClass<UCameraComponent>())
		{
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());
			Params.AddIgnoredActor(TargetComponent->GetOwner());
			FHitResult Result;
			if (!GetWorld()->LineTraceSingleByChannel(Result, Camera->GetComponentLocation(), TargetComponent->GetComponentLocation(), ProbeChannel, Params))
			{
				return;
			}
		}
		
	}

	SetTarget(nullptr);
	GetWorld()->GetTimerManager().ClearTimer(ChechTargetTimer);
}

