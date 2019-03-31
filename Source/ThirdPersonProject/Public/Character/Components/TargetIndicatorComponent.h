// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TargetIndicatorComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THIRDPERSONPROJECT_API UTargetIndicatorComponent : public USceneComponent
{
	GENERATED_BODY()	
	
public:
	UTargetIndicatorComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void TargetChanged(class UTargetComponent* NewTarget);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Scale)
	FVector2D DistanceDiapason = FVector2D(50.0f, 1000.0f);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Scale)
	FVector2D ScaleDiapason = FVector2D(0.1f, 1.0f);

	UPROPERTY(BlueprintReadOnly)
	class UTargetComponent* Target;

	UPROPERTY(BlueprintReadOnly)
	class UCameraComponent* Camera;	
};
