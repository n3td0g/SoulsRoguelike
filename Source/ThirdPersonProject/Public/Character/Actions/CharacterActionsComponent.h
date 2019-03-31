// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionType.h"
#include "Components/ActorComponent.h"
#include "CharacterActionsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONPROJECT_API UCharacterActionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterActionsComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool StartAction(EActionType NewActionType);

	UFUNCTION(BlueprintCallable)
	bool StopAction(EActionType ActionType);

    UFUNCTION(BlueprintCallable)
    UBaseAction* GetPrevAction();

    UFUNCTION(BlueprintCallable)
	bool StopCurrentAction();

    UFUNCTION(BlueprintPure)
	FORCEINLINE UBaseAction* GetCurrentAction() { return CurrentAction; }

    UFUNCTION(BlueprintPure)
    FORCEINLINE EActionType GetNextActionType() { return NextAction; }

public:
    UPROPERTY(BlueprintAssignable)
    FOnStopActionSignature OnActionStopped;

    UPROPERTY(BlueprintAssignable)
    FOnBeforeStopActionSignature OnBeforeStopAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnStopAction(UBaseAction* Action);

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = Actions)
    TArray<UBaseAction*> Actions;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Actions)
    float NextActionMemoryTime = 0.75f;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Actions)
    float PrevActionMemoryTime = 0.75f;

	UPROPERTY(BlueprintReadOnly)
	TMap<EActionType, UBaseAction*> CharacterActions;

	UPROPERTY(BlueprintReadOnly)
	UBaseAction* CurrentAction;

    UPROPERTY(BlueprintReadOnly)
    UBaseAction* PrevAction;

private:
	void CreateActions();

private:
    UPROPERTY(Transient)
    TArray<UBaseAction*> ActionsToTick;

    EActionType NextAction = EActionType::None;

    float NextActionActivationTime = 0.0f;
    float PrevActionTime = 0.0f;
};
