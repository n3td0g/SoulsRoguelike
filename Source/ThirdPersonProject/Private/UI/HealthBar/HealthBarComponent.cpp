// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBarComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterStatsComponent.h"
#include "HealthBarWidget.h"
#include "ThirdPersonProjectCharacter.h"

UHealthBarComponent::UHealthBarComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetHiddenInGame(true);
}

void UHealthBarComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bHiddenInGame)
    {
        if (IsValid(LocalCharacter))
        {
            const FVector& CameraLocation = LocalCharacter->GetFollowCamera()->GetComponentLocation();
            FVector DirectionToCamera = CameraLocation - GetComponentLocation();
            float DistanceToCamera = DirectionToCamera.Size();
            DirectionToCamera /= DistanceToCamera;

            SetWorldRotation(DirectionToCamera.ToOrientationRotator());

            float Scale = FMath::GetMappedRangeValueClamped(FVector2D(MinScaleDistance, MaxScaleDistance), FVector2D(MinScale, MaxScale), DistanceToCamera);
            SetWorldScale3D(FVector(Scale));
        }
    }
}

void UHealthBarComponent::BeginPlay()
{
    Super::BeginPlay();

    if (auto OwnerCharacter = Cast<ABaseCharacter>(GetOwner()))
    {
        OwnerCharacter->GetStatsComponent()->OnValueChanged.AddDynamic(this, &UHealthBarComponent::OnHealthValueChanged);
        if (auto HealthBar = Cast<UHealthBarWidget>(GetUserWidgetObject()))
        {
            HealthBar->SetCharacter(OwnerCharacter);
        }
    }

    LocalCharacter = Cast<AThirdPersonProjectCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void UHealthBarComponent::OnHealthValueChanged(float NewValue, EStatsType InStatType)
{
    if (InStatType == EStatsType::Health)
    {
        if (NewValue <= 0.0f)
        {
            SetHiddenInGame(true);
            return;
        }
        SetHiddenInGame(false);
        GetWorld()->GetTimerManager().SetTimer(HideWidgetTimer, this, &UHealthBarComponent::HideComponent, HideComponentDelay, false);
    }
}

void UHealthBarComponent::HideComponent()
{
    SetHiddenInGame(true);
}
