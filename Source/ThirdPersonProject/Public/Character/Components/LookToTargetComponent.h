// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TargetComponent.h"
#include "LookToTargetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetChangedDelegate, UTargetComponent*, NewTarget);

class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONPROJECT_API ULookToTargetComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULookToTargetComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetTarget(UTargetComponent* NewTarget);

	UFUNCTION(BlueprintCallable)
	void SelectNearestTarget(float MaxCos = 0.5f, float MaxDistance = 1500.0f);

	UFUNCTION(BlueprintCallable)
	void SelectNextTarget(float Sign, float MaxCos = 0.5f, float MaxDistance = 1200.0f);

	virtual void SetActive(bool bNewActive, bool bReset) override;

public:
	UPROPERTY(BlueprintAssignable)
	FTargetChangedDelegate OnTargetChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	//Offset
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Offset")
	FVector2D OffsetDistance = FVector2D(50.0f, 1000.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Offset")
	FVector2D OffsetValue = FVector2D(-50.0f, 200.0f);

	UPROPERTY(BlueprintReadOnly)
	UTargetComponent* TargetComponent;
	
	UPROPERTY(BlueprintReadOnly)
	ABaseCharacter* OwnerCharacter;

	//Indicator
	UPROPERTY(BlueprintReadWrite)
	UPrimitiveComponent* IndicatorComponent;

	//Limits
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Limits)
	TEnumAsByte<ECollisionChannel> ProbeChannel = ECollisionChannel::ECC_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Limits)
	float MaxDistanceToTarget = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Limits)
	float CheckTargetRate = 1.0f;

private:
	UFUNCTION()
	void CheckTarget();

private:
	FTimerHandle ChechTargetTimer;
};
