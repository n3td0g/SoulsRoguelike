// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatsType.h"
#include "BaseCharacter.generated.h"

class UCharacterActionsComponent;
class UCharacterStatsComponent;
class ABaseWeapon;

UCLASS()
class THIRDPERSONPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

public:
    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE UCharacterActionsComponent* GetActionComponent() { return ActionComponent;  }
	FORCEINLINE UCharacterStatsComponent* GetStatsComponent() { return StatsComponent; }

    UFUNCTION(BlueprintCallable)
    ABaseWeapon* GetCurrentWeapon();

    //Stats
    UFUNCTION(BlueprintCallable)
    void ChangeStatMaxValue(EStatsType Type, float NewMaxValue);

    UFUNCTION(BlueprintCallable)
    float ChangeStatValue(EStatsType Type, float Delta);

    UFUNCTION(BlueprintCallable)
    bool TryToChangeStatValue(EStatsType Type, float Delta);

    UFUNCTION(BlueprintPure)
    FStatData& GetStatData(EStatsType Type);

    UFUNCTION(BlueprintPure)
    float GetDataValue(EStatsType Type);
    //----------------------------------

    //Actions
    UFUNCTION(BlueprintCallable)
    bool StartAction(EActionType NewActionType);

    UFUNCTION(BlueprintCallable)
    bool StopAction(EActionType ActionType);

    UFUNCTION(BlueprintCallable)
    UBaseAction* GetPrevAction();

    UFUNCTION(BlueprintCallable)
    bool StopCurrentAction();

    UFUNCTION(BlueprintPure)
    UBaseAction* GetCurrentAction();

    UFUNCTION(BlueprintPure)
    EActionType GetNextActionType();
    //----------------------------------

	//Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Components)
	UCharacterActionsComponent* ActionComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Components)
	UCharacterStatsComponent* StatsComponent;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Weapons)
    TSubclassOf<ABaseWeapon> TestWeaponClass;

    UPROPERTY(BlueprintReadOnly)
    ABaseWeapon* TestWeapon;

    float _takeOffTime = 0.0f;
    float _takeOffDistance = 0.0f;
    float _takeOffVelocity = 1.0f;
};
