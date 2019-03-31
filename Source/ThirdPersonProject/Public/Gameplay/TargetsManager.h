// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetsManager.generated.h"

class UTargetComponent;

UCLASS()
class THIRDPERSONPROJECT_API ATargetsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetsManager();

	UFUNCTION(BlueprintPure)
	UTargetComponent* GetNearestTarget(const FVector& Location, const FVector& Right, const FVector& Forward, TEnumAsByte<ECollisionChannel> TraceChannel,
		UTargetComponent* CurrentTarget = nullptr, float MinCos = 0.5f, float MaxDistance = 1500.0f);

	UFUNCTION(BlueprintCallable)
	void AddTarget(UTargetComponent* NewTarget);

	UFUNCTION(BlueprintCallable)
	void RemoveTarget(UTargetComponent* TargetToRemove);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UTargetComponent*> Targets;
};
