// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    for (uint8 I = 0; I < (uint8)EAttackType::Count; ++I)
    {
        WeaponAnimations.Add((EAttackType)I, FWeaponAnimations());
    }
}

void ABaseWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABaseWeapon::Equip(USceneComponent* ComponentAttachTo)
{
    ReceiveEquip(ComponentAttachTo);

    if (ComponentAttachTo)
    {
        AttachToComponent(ComponentAttachTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetSocketToAttach());
    }
}

void ABaseWeapon::Unequip(USceneComponent* ComponentAttachTo)
{
    ReceiveUnequip(ComponentAttachTo);
}

void ABaseWeapon::StartAttack(EAttackType AttackType)
{
    CurrentWeaponAnimation = WeaponAnimations.Find(AttackType);

    ReceiveStartAttack();
}

void ABaseWeapon::StopAttack()
{
    ReceiveStopAttack();

    CurrentWeaponAnimation = nullptr;
}

UAnimMontage* ABaseWeapon::GetAttackMontage(EAttackType AttackType, int32& NumOfCombo)
{
    if (auto Result = WeaponAnimations.Find(AttackType))
    {
        if (Result->AttackAnimation)
        {
            NumOfCombo = Result->NumOfCombo;
            return Result->AttackAnimation;
        }
    }

    return DefaultMontage;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

