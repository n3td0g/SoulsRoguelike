// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "TPGameplayStatics.generated.h"

class ATargetsManager;
/**
 * 
 */
UCLASS()
class THIRDPERSONPROJECT_API UTPGameplayStatics : public UGameplayStatics
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Targets", meta = (WorldContext = "WorldContextObject"))
	static class ATargetsManager* GetTargetsManager(const UObject* WorldContextObject);
	
};
