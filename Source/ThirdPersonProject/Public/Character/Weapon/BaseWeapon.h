// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    Default,
    AfterRun,
    OnFalling,
    Count UMETA(Hidden),
    None  UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FWeaponAnimations
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Animation)
    UAnimMontage* AttackAnimation;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Animation, meta = (ClampMin = "1", ClampMax = "100"))
    int32 NumOfCombo = 1;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Animation)
    UAnimMontage* HeavyAttackAnimation;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Animation)
    bool bUseControllerRotation = false;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Damage)
    float Damage = 20.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Stamina)
	float RequiredStamina = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Rotation)
	float YawRotationRate = 0.0f;
};

UCLASS(BlueprintType, Blueprintable)
class THIRDPERSONPROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
    ABaseWeapon();

public:
	virtual void Tick(float DeltaTime) override;

    virtual void Equip(USceneComponent* ComponentAttachTo);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "Equip"))
    void ReceiveEquip(USceneComponent* ComponentAttachTo);
    
    virtual void Unequip(USceneComponent* ComponentAttachTo);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "Unequip"))
    void ReceiveUnequip(USceneComponent* ComponentAttachTo);

    virtual void StartAttack(EAttackType AttackType);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "OnStartAttack"))
    void ReceiveStartAttack();

    virtual void StopAttack();
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "OnStopAttack"))
    void ReceiveStopAttack();

    UFUNCTION(BlueprintCallable)
    UAnimMontage* GetAttackMontage(EAttackType AttackType, int32& NumOfCombo, float& RequiredStamina, float& OutRotationRate);

public:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ETraceTypeQuery> ChannelToTrace;

protected:
    virtual void BeginPlay() override;

    virtual FName& GetSocketToAttach() { return SocketToAttach; }

protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Attach)
    FName SocketToAttach;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Animation)
    UAnimMontage* DefaultMontage;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, EditFixedSize, Category = Anumation)
    TMap<EAttackType, FWeaponAnimations> WeaponAnimations;

    FWeaponAnimations *CurrentWeaponAnimation = nullptr;
};
