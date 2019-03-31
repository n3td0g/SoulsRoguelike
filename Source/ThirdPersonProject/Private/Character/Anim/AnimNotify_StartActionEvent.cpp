// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_StartActionEvent.h"
#include "CharacterAnimInstance.h"

UAnimNotify_StartActionEvent::UAnimNotify_StartActionEvent()
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor(0, 150, 150, 255);
#endif // WITH_EDITORONLY_DATA
}

void UAnimNotify_StartActionEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    check(MeshComp);

    if (auto AnimInstance = Cast<UCharacterAnimInstance>(MeshComp->GetAnimInstance()))
    {
        AnimInstance->StartActionEvent();
    }
}
