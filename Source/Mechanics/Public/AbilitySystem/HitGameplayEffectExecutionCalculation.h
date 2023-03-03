// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "HitGameplayEffectExecutionCalculation.generated.h"

/**
 * ExecutionCalculations can only be used with Instant and Periodic GameplayEffects, can not be predicted
 */
UCLASS()
class MECHANICS_API UHitGameplayEffectExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	UHitGameplayEffectExecutionCalculation();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
