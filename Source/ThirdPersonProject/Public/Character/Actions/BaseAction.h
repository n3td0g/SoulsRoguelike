// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionType.h"
#include "BaseAction.generated.h"

class ACharacter;
class ABaseCharacter;

/**
 * 
 */
UCLASS(Blueprintable)
class THIRDPERSONPROJECT_API UBaseAction : public UObject
{
	GENERATED_BODY()
	
public:
	bool HasHigherPriority(EActionType PrevActionType);

    virtual UWorld* GetWorld() const override;
	virtual void Init();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Init"))
	void ReceiveInit();

	virtual void ActionTick(float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
	void ReceiveTick(float DeltaTime);

	virtual bool Activate();

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Activate"))
	bool ReceiveActivate();
	
	virtual bool StopAction(bool bIsForce);

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "StopAction"))
	bool ReceiveStopAction(bool bIsForce);

    virtual bool ContinueAction();

	void SetOwner(AActor* Owner);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EActionType GetActionType() const { return Type; }

    FORCEINLINE bool GetIsActive() { return bIsActive; }

    FORCEINLINE bool GetTickOnlyIfActive() { return bTickOnlyIfActive; }

	static FString GetActionTypeAsString(EActionType ActionType);

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnStopActionSignature OnStopAction;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Action)
	EActionType Type = EActionType::None;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Action)
    bool bTickOnlyIfActive = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Action, meta = (Bitmask, BitmaskEnum = "EActionType"))
	int32 ActionsThatCanBeBraked;

	UPROPERTY(BlueprintReadOnly)
	ABaseCharacter* OwnerCharacter;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

private:
    virtual bool CanBeStopped();
};
