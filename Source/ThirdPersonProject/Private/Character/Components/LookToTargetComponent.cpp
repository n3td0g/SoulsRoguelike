// Fill out your copyright notice in the Description page of Project Settings.

#include "LookToTargetComponent.h"
#include "BaseCharacter.h"
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

	if(!OwnerCharacter || !OwnerCharacter->GetController())
	{
		return;
	}

	if (IsValid(TargetComponent))
	{
		if (!TargetComponent->IsActive())
		{
			SetTarget(nullptr);
			SelectNearestTarget();
			if (!IsValid(TargetComponent))
			{
				return;
			}
		}

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
		
		OwnerCharacter->GetController()->SetControlRotation(TargetRotation);

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
	if (auto MovementComponent = OwnerCharacter->GetCharacterMovement())
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
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (auto Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>())
	{
		if (auto TargetsManager = UTPGameplayStatics::GetTargetsManager(this))
		{
			SetTarget(TargetsManager->GetNearestTarget(OwnerCharacter->GetActorLocation(), FVector::ZeroVector, Camera->GetForwardVector(), ProbeChannel, nullptr, MaxCos, MaxDistance));
		}
	}
}

void ULookToTargetComponent::SelectNextTarget(float Sign, float MaxCos /*= 0.5f*/, float MaxDistance /*= 1500.0f*/)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (!IsValid(TargetComponent))
	{
		return;
	}

	if (auto Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>())
	{
		if (auto TargetsManager = UTPGameplayStatics::GetTargetsManager(this))
		{
			auto NextTarget = TargetsManager->GetNearestTarget(OwnerCharacter->GetActorLocation(), Camera->GetRightVector() * Sign,
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

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}

void ULookToTargetComponent::CheckTarget()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (IsValid(TargetComponent))
	{
		if (auto Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>())
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

