// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_StopAction.h"
#include "CharacterAnimInstance.h"

UAnimNotify_StopAction::UAnimNotify_StopAction()
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor(200, 100, 100, 255);
#endif // WITH_EDITORONLY_DATA
}

void UAnimNotify_StopAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    check(MeshComp);

    if (auto AnimInstance = Cast<UCharacterAnimInstance>(MeshComp->GetAnimInstance()))
    {
        AnimInstance->StopCurrentAction();
    }
}
