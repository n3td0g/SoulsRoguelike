// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsType.h"
#include "CharacterStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatValueChanged, float, NewValue, EStatsType, StatType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONPROJECT_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComponent();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ChangeStatMaxValue(EStatsType Type, float NewMaxValue);

	UFUNCTION(BlueprintCallable)
	float ChangeStatValue(EStatsType Type, float Delta);

    UFUNCTION(BlueprintCallable)
    bool TryToChangeStatValue(EStatsType Type, float Delta);

	UFUNCTION(BlueprintPure)
	FStatData& GetStatData(EStatsType Type);

	UFUNCTION(BlueprintPure)
	float GetDataValue(EStatsType Type) const;

	UFUNCTION(BlueprintCallable)
	void SetRecoveryBlock(bool bBlockRecovery, EStatsType Type);
public:
    UPROPERTY(BlueprintAssignable)
    FOnStatValueChanged OnValueChanged;

    UPROPERTY(BlueprintAssignable)
    FOnStatValueChanged OnMaxValueChanged;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditFixedSize, EditAnywhere, Category = Stats)
	TArray<FStatData> Stats;

	TArray<FStatData*> StatsToRecovery;
private:
	void AddToRecoveryList(FStatData& Data);

    void ChangeStatValue(FStatData& Data, float Delta);

	void ChangeStatDataValue(FStatData& Data, float Delta);
};
