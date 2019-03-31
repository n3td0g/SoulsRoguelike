// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "BaseAction.h"
#include "StartActionAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionOutputPin);

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UStartActionAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:    
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Actions")
    static UStartActionAsync* StartActionAsync(UObject* WorldContextObject, class ABaseCharacter* InCharacter, EActionType InActionType);

    virtual void Activate() override;
	
public:
    UPROPERTY(BlueprintAssignable)
    FActionOutputPin OnBeforeStop;

    UPROPERTY(BlueprintAssignable)
    FActionOutputPin OnSuccess;

    UPROPERTY(BlueprintAssignable)
    FActionOutputPin OnFailed;
protected:
    UFUNCTION()
    void OnBeforeStopAction(UBaseAction* Action);

    UFUNCTION()
    void OnActionStopped(UBaseAction* Action);

protected:
    UPROPERTY()
    class ABaseCharacter* Character;

    EActionType ActionType;
};
