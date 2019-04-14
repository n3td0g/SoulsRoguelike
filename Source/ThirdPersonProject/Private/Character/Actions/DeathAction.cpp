// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathAction.h"
#include "Classes/AIController.h"
#include "Classes/BrainComponent.h"
#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "CharacterActionsComponent.h"
#include "CharacterStatsComponent.h"
#include "LookToTargetComponent.h"
#include "TargetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UDeathAction::UDeathAction()
{
    Type = EActionType::Death;
}

bool UDeathAction::Activate()
{
    if (!Super::Activate())
    {
        return false;
    }

    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->GetCharacterMovement()->Deactivate();
        OwnerCharacter->GetStatsComponent()->Deactivate();
		TArray<UActorComponent*> TargetComponents = OwnerCharacter->GetComponentsByClass(UTargetComponent::StaticClass());
		for (UActorComponent* Component : TargetComponents)
		{
			if (UTargetComponent *Target = Cast<UTargetComponent>(Component))
			{
				Target->Deactivate();
			}
		}
		if (ULookToTargetComponent* LookToTarget = OwnerCharacter->FindComponentByClass<ULookToTargetComponent>())
		{
			LookToTarget->Deactivate();
		}

		if (AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController()))
		{
			AIController->GetBrainComponent()->StopLogic(TEXT("Character is dead"));
		}
    }
    return true;
}

bool UDeathAction::StopAction(bool bIsForce)
{
    Super::StopAction(bIsForce);
    
    if (IsValid(OwnerCharacter))
    {
        if (auto CharacterMesh = OwnerCharacter->GetMesh())
        {
            CharacterMesh->SetAnimInstanceClass(nullptr);
            CharacterMesh->SetAnimationMode(EAnimationMode::AnimationCustomMode);

            CharacterMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
            CharacterMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
            CharacterMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
            CharacterMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
            CharacterMesh->SetSimulatePhysics(true);
        }
        OwnerCharacter->GetActionComponent()->Deactivate();
        OwnerCharacter->GetCapsuleComponent()->SetSimulatePhysics(false);
        OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    return false;
}

bool UDeathAction::CanBeStopped()
{
    return false;
}
