// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ThirdPersonProjectCharacter.generated.h"

class UBaseAction;
class ABaseWeapon;

UCLASS(config=Game)
class AThirdPersonProjectCharacter : public ABaseCharacter
{
	GENERATED_BODY()	

public:
	AThirdPersonProjectCharacter();

    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    UFUNCTION(BlueprintCallable)
    void SwitchWeapon(float Sign);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Run)
    float TimeToRoll;

protected:
    virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

    void RunButtonPressed();
    void RunButtonReleased();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Weapon)
    TArray<TSubclassOf<ABaseWeapon>> PlayerWeaponClasses;

private:
    UFUNCTION()
    void OnActionStopped(UBaseAction* CurrentAction);

    void TryToSwitchWeapon(int32 NewWeaponIndex);

private:
    //Inpit
    float LastRunInputTime = 0.0f;
    bool bIsRunButtonPressed = false;
    int32 CurrentWeaponIndex = -1;
};

