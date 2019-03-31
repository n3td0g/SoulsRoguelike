// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_StopActionEven.h"
#include "CharacterAnimInstance.h"

UAnimNotify_StopActionEven::UAnimNotify_StopActionEven()
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor(150, 0, 150, 255);
#endif // WITH_EDITORONLY_DATA
}

void UAnimNotify_StopActionEven::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    check(MeshComp);

    if (auto AnimInstance = Cast<UCharacterAnimInstance>(MeshComp->GetAnimInstance()))
    {
        AnimInstance->StopActionEvent();
    }
}
