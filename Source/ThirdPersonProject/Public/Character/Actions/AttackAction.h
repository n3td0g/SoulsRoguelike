// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Character/Actions/BaseMontageAction.h"
#include "AttackAction.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UAttackAction : public UBaseMontageAction
{
	GENERATED_BODY()
	
public:
    UAttackAction();
	
	virtual void Init() override;
    virtual bool ContinueAction() override;
    virtual bool Activate() override;
    virtual bool StopAction(bool bIsForce) override;

protected:
    virtual void StartAnimationEvent() override;
    virtual void StopAnimationEvent() override;

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Combo)
    FString ComboNamePrefix = TEXT("Combo_");

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Trace)
	TEnumAsByte<ETraceTypeQuery> ChannelToTrace;

    UPROPERTY(BlueprintReadOnly)
    class ABaseWeapon* CurrentWeapon;

private:
    EAttackType GetAttackType() const;
    bool IsOnRunAttack() const;
private:
    //Combo
    int32 ComboIndex = 0;
    EAttackType CurrAttackType = EAttackType::None;
};
