// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/AbilitySystem/HitGameplayEffectExecutionCalculation.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "AbilitySystemComponent.h"

struct HandDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FightSkill);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	HandDamageStatics()
	{
		// Do not snapshot if we want to use the value at the execution moment, (value at the end = false)
		// we snapshot if we want to use the value at the creation moment. (value at the start = true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Strength, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, FightSkill, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Health, Target, false);
	}
};

static const HandDamageStatics& DamageStatics()
{
	static HandDamageStatics DamageStatics;
	return DamageStatics;
}

static void CaptureMagnitudeOrDefault(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectAttributeCaptureDefinition& Def, const FAggregatorEvaluateParameters& EvaluationParameters,
	float& Value, float defaultValue)
{
	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Def, EvaluationParameters, Value))
	{
		Value = defaultValue;
	}
}

UHitGameplayEffectExecutionCalculation::UHitGameplayEffectExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(DamageStatics().FightSkillDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
}

void UHitGameplayEffectExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	
	// We can assign the magnitude value dynamically via spec handle by using Assign Tag Set by Caller Magnitude
	const float BaseDamage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("BaseDamage")), false, 0);
	UE_LOG(LogTemp, Warning, TEXT("Execute_Implementation %f"), BaseDamage);
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Strength = 0.f;
	CaptureMagnitudeOrDefault(ExecutionParams, DamageStatics().StrengthDef, EvaluationParameters, Strength, 0.0f);
	
	float FightSkill = 0.f;
	CaptureMagnitudeOrDefault(ExecutionParams, DamageStatics().FightSkillDef, EvaluationParameters, FightSkill, 0.0f);

	const float Damage = (Strength / 100.0f + FightSkill / 100.0f) * BaseDamage;
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, Damage));
}
