// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void SetCharacter(class ABaseCharacter* Character);

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
    class UStatProgressBar* HealthBar;
    
};
