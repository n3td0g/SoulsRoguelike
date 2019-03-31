// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

void AMeleeWeapon::StartAttack(EAttackType AttackType)
{
    Super::StartAttack(AttackType);

    if (!CurrentWeaponAnimation)
    {
        return;
    }

    ActorsToIgnore.Empty();
    ActorsToIgnore.Add(GetOwner());

    UpdatePrevTracePoints();
    GetWorld()->GetTimerManager().SetTimer(TraceTimer, this, &AMeleeWeapon::Trace, TraceUpdateTime, true, TraceUpdateTime);
}

void AMeleeWeapon::StopAttack()
{
    Super::StopAttack();

    GetWorld()->GetTimerManager().ClearTimer(TraceTimer);
}

void AMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();

    auto SceneComponents = GetComponentsByTag(USceneComponent::StaticClass(), ComponentToTraceTag);
    for (auto SceneComponent : SceneComponents)
    {
        if (auto ComponentToTrace = Cast<USceneComponent>(SceneComponent))
        {
            ComponentsToTrace.Add(ComponentToTrace);
        }
    }
}

void AMeleeWeapon::Trace()
{
    if (!CurrentWeaponAnimation)
    {
        return;
    }

    FHitResult HitResult;
    int32 Index = 0;   

    for (auto ComponentToTrace : ComponentsToTrace)
    {
        EDrawDebugTrace::Type DebugTrace = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
        if(UKismetSystemLibrary::SphereTraceSingle(this, ComponentToTrace->GetComponentLocation(), PrevTracePoint[Index++], TraceRadius, 
            ChannelToTrace, false, ActorsToIgnore, DebugTrace, HitResult, false, FLinearColor::Red))
        {
            if (bDebug)
            {
                ::DrawDebugBox(GetWorld(), HitResult.TraceStart, FVector(5.0f, 5.0f, 5.0f), FColor::Blue);
            }

            if (HitResult.GetActor())
            {
                FPointDamageEvent DamageEvent(CurrentWeaponAnimation->Damage, HitResult, HitResult.ImpactPoint, UDamageType::StaticClass());
                AController* OwnerController = nullptr;
                if (auto Pawn = Cast<APawn>(GetOwner()))
                {
                    OwnerController = Pawn->GetController();
                }
                HitResult.GetActor()->TakeDamage(CurrentWeaponAnimation->Damage, DamageEvent, OwnerController, this);
                ActorsToIgnore.Add(HitResult.GetActor());
            }
        }
    }

    UpdatePrevTracePoints();
}

void AMeleeWeapon::UpdatePrevTracePoints()
{
    PrevTracePoint.Empty();
    for (auto ComponentToTrace : ComponentsToTrace)
    {
        PrevTracePoint.Add(ComponentToTrace->GetComponentLocation());
    }
}
