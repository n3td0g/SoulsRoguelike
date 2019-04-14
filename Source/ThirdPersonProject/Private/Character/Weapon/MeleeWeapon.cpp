// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

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

	UGameplayStatics::PlaySoundAtLocation(this, SwingSound, GetActorLocation());
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
	bool bAnyHit = false;
	FVector LastHitLocation(FVector::ZeroVector);
	FVector LastHitDirecton(FVector::ZeroVector);

    for (auto ComponentToTrace : ComponentsToTrace)
    {
        EDrawDebugTrace::Type DebugTrace = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		const FVector& PrevTraceLocation = PrevTracePoint[Index++];
        if(UKismetSystemLibrary::SphereTraceSingle(this, ComponentToTrace->GetComponentLocation(), PrevTraceLocation, TraceRadius,
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
				bAnyHit = true;
				LastHitLocation = HitResult.Location;
				LastHitDirecton = ComponentToTrace->GetComponentLocation() - PrevTraceLocation;
				
            }
        }
    }

	if (bAnyHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, LastHitLocation);
		LastHitDirecton.Normalize();
		const FRotator& EffectRotation = UKismetMathLibrary::MakeRotFromX(LastHitDirecton);
		UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, LastHitLocation, EffectRotation);
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
