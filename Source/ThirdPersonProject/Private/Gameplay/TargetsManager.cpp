// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetsManager.h"
#include "TargetComponent.h"

// Sets default values
ATargetsManager::ATargetsManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

UTargetComponent* ATargetsManager::GetNearestTarget(const FVector& Location, const FVector& Right, const FVector& Forward, TEnumAsByte<ECollisionChannel> TraceChannel,
	UTargetComponent* CurrentTarget /*= nullptr*/, float MinCos /*= 0.5f*/, float MaxDistance /*= 1500.0f*/)
{
	UTargetComponent* ResultTarget = nullptr;
	float MaxDistanceSquared = MaxDistance * MaxDistance;

	float MinSin = 1.0f;
	for (int32 I = Targets.Num() - 1; I >= 0; --I)
	{
		UTargetComponent* Target = Targets[I];

		if (!IsValid(Target))
		{
			Targets.RemoveAt(I);
			continue;
		}

		if (!Target->IsActive())
		{
			continue;
		}

		if (Target == CurrentTarget)
		{
			continue;
		}

		const FVector& TargetLocation = Target->GetComponentLocation();
		FVector VectorToTarget = TargetLocation - Location;
		float DistanceToTargetSquared = VectorToTarget.SizeSquared();
		if (DistanceToTargetSquared > MaxDistanceSquared)
		{
			continue;
		}

		VectorToTarget /= FMath::Sqrt(DistanceToTargetSquared);

		float Cos = FVector::DotProduct(VectorToTarget, Forward);
		if (Cos > MinCos)
		{
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Target->GetOwner());

			FHitResult Result;
			if (GetWorld()->LineTraceSingleByChannel(Result, Location, TargetLocation, TraceChannel, Params))
			{
				continue;
			}

			if (IsValid(CurrentTarget))
			{				
				float Sin = FVector::DotProduct(VectorToTarget, Right);
				if (Sin >= 0.0f && Sin < MinSin)
				{
					MinSin = Sin;
					ResultTarget = Target;
				}			
			}
			else
			{
				MinCos = Cos;
				ResultTarget = Target;
			}
		}
	}

	return ResultTarget;
}

void ATargetsManager::AddTarget(UTargetComponent* NewTarget)
{
	Targets.Add(NewTarget);
}

void ATargetsManager::RemoveTarget(UTargetComponent* TargetToRemove)
{
	Targets.Remove(TargetToRemove);
}

// Called when the game starts or when spawned
void ATargetsManager::BeginPlay()
{
	Super::BeginPlay();
	
}

