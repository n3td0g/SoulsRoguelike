// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "StartActionAsync.h"
#include "CharacterActionsComponent.h"
#include "CharacterStatsComponent.h"
#include "BaseWeapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ActionComponent = CreateDefaultSubobject<UCharacterActionsComponent>("ActionComponent");
	StatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>("StatsComponent");
}

float ABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    
    if (StatsComponent->ChangeStatValue(EStatsType::Health, -ActualDamage) <= 0.0f)
    {
        ActionComponent->StartAction(EActionType::Death);
    }
    return ActualDamage;
}

ABaseWeapon* ABaseCharacter::GetCurrentWeapon()
{
    if (!IsValid(TestWeapon))
    {
        if (TestWeaponClass)
        {
            FActorSpawnParameters Params;
            Params.Owner = this;
            Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            TestWeapon = GetWorld()->SpawnActor<ABaseWeapon>(TestWeaponClass, Params);
            TestWeapon->Equip(GetMesh());
        }
    }

    return TestWeapon;
}

//--------------Stats--------------
void ABaseCharacter::ChangeStatMaxValue(EStatsType Type, float NewMaxValue)
{
    StatsComponent->ChangeStatMaxValue(Type, NewMaxValue);
}

float ABaseCharacter::ChangeStatValue(EStatsType Type, float Delta)
{
    return StatsComponent->ChangeStatValue(Type, Delta);
}

bool ABaseCharacter::TryToChangeStatValue(EStatsType Type, float Delta)
{
    return StatsComponent->TryToChangeStatValue(Type, Delta);
}

FStatData& ABaseCharacter::GetStatData(EStatsType Type)
{
    return StatsComponent->GetStatData(Type);
}

float ABaseCharacter::GetDataValue(EStatsType Type)
{
    return StatsComponent->GetDataValue(Type);
}

//------------------------------------
//--------------Actions---------------

bool ABaseCharacter::StartAction(EActionType NewActionType)
{
    return ActionComponent->StartAction(NewActionType);
}

bool ABaseCharacter::StopAction(EActionType ActionType)
{
    return ActionComponent->StopAction(ActionType);
}

UBaseAction* ABaseCharacter::GetPrevAction()
{
    return ActionComponent->GetPrevAction();
}

bool ABaseCharacter::StopCurrentAction()
{
    return ActionComponent->StopCurrentAction();
}

UBaseAction* ABaseCharacter::GetCurrentAction()
{
    return ActionComponent->GetCurrentAction();
}

EActionType ABaseCharacter::GetNextActionType()
{
    return ActionComponent->GetNextActionType();
}

//------------------------------------

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    GetCurrentWeapon();
}
