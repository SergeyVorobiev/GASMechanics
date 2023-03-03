// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MyAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UMyAttributeSet::UMyAttributeSet()
{
}

void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Xp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, FightSkill, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Speed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxLevel, COND_None, REPNOTIFY_Always);
}

void UMyAttributeSet::InitFromMetaDataTable(const UDataTable* DataTable)
{
	Super::InitFromMetaDataTable(DataTable);
}

void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Health, OldHealth);
}

void UMyAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxHealth, OldMaxHealth);
}

void UMyAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Mana, OldMana);
}

void UMyAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxMana, OldMaxMana);
}

void UMyAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Level, OldLevel);
}

void UMyAttributeSet::OnRep_MaxLevel(const FGameplayAttributeData& OldMaxLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxLevel, OldMaxLevel);
}

void UMyAttributeSet::OnRep_Xp(const FGameplayAttributeData& OldXp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Xp, OldXp);
}

void UMyAttributeSet::OnRep_MaxXp(const FGameplayAttributeData& OldMaxXp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxXp, OldMaxXp);
}

void UMyAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Stamina, OldStamina);
}

void UMyAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxStamina, OldMaxStamina);
}

void UMyAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Strength, OldStrength);
}

void UMyAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Speed, OldSpeed);
}

void UMyAttributeSet::OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxSpeed, OldMaxSpeed);
}

void UMyAttributeSet::OnRep_FightSkill(const FGameplayAttributeData& OldFightSkill)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, FightSkill, OldFightSkill);
}

void UMyAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("PreAttributeBaseChange (%s) value: %f"), *Attribute.GetName(), NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UMyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("PreAttributeChange (%s) value: %f"), *Attribute.GetName(), NewValue);
}

void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		PostStaminaEffectExecutor(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		PostHealthEffectExecutor(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		PostMaxHealthEffectExecutor(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		PostManaEffectExecutor(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetXpAttribute())
	{
		PostXpEffectExecutor(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetSpeedAttribute())
	{
		PostSpeedEffectExecutor(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetMaxSpeedAttribute())
	{
		PostMaxSpeedEffectExecutor(Data);
	}
	else if (Data.EvaluatedData.Attribute == GetLevelAttribute())
	{
		PostLevelEffectExecutor(Data);
	}
	UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute: %s"), *Data.EvaluatedData.Attribute.GetName());
}

// Used to modify or discard the values passed into an effect execution.
bool UMyAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	const bool Result = Super::PreGameplayEffectExecute(Data); // Current implementation will always return true
	UE_LOG(LogTemp, Warning, TEXT("PreGameplayEffectExecute"));
	if (Result && Data.EvaluatedData.Attribute == GetStaminaAttribute() && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		if (Data.EvaluatedData.Magnitude >= 0)
		{
			return GetStamina() < GetMaxStamina();
		} 
		return GetStamina() > 0;
	}
	if (Result && Data.EvaluatedData.Attribute == GetManaAttribute() && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		if (Data.EvaluatedData.Magnitude >= 0)
		{
			return GetMana() < GetMaxMana();
		} 
		return GetMana() > 0;
	}
	return Result;
}

void UMyAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxHealth.GetBaseValue());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxMana.GetBaseValue());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxStamina.GetBaseValue());
	}
	else if (Attribute == GetXpAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxXp.GetBaseValue());
	}
	else if (Attribute == GetLevelAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxLevel.GetBaseValue());
	}
}
	
void UMyAttributeSet::PostStaminaEffectExecutor(const FGameplayEffectModCallbackData& Data)
{
	const auto Actor = Cast<ACharacterBase>(GetOwningActor());
	if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		const float FCurStamina = GetStamina();
		const float FMaxStamina = GetMaxStamina();
		Actor->OnAttributeChanged(AttributeChange::Stamina, FCurStamina, FMaxStamina, Data.EvaluatedData.Magnitude);
		UE_LOG(LogTemp, Warning, TEXT("PostStaminaEffectExecutor %f - %f"), FCurStamina, FMaxStamina);
	}
	else if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Override) // Currently just think that it is happened due to level up.
	{
		Actor->OnAttributeChanged(AttributeChange::Stamina, GetStamina(), GetMaxStamina(), Data.EvaluatedData.Magnitude);
	}
}

void UMyAttributeSet::PostHealthEffectExecutor(const FGameplayEffectModCallbackData& Data)
{
	const auto Actor = Cast<ACharacterBase>(GetOwningActor());
	if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		const float FCurHealth = GetHealth();
		const float FMaxHealth = GetMaxHealth();
		const auto Instigator = Cast<ACharacterBase>(Data.EffectSpec.GetEffectContext().GetInstigator());
		if (Instigator && Data.EvaluatedData.Magnitude < 0)
		{
			Instigator->OnMakeDamage(Data.EvaluatedData.Magnitude);
		}
		Actor->OnAttributeChanged(AttributeChange::Health, FCurHealth, FMaxHealth, Data.EvaluatedData.Magnitude);
		// FMath::IsNearlyEqual(FCurHealth, 0)
		Actor->OnHealthChanged(FCurHealth, Data.EvaluatedData.Magnitude, FCurHealth < 1.0f);
		UE_LOG(LogTemp, Warning, TEXT("PostHealthEffectExecutor %f - %f: %f"), FCurHealth, FMaxHealth, Data.EvaluatedData.Magnitude);
	}
	else if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Override) // Currently just think that it is happened due to level up.
	{
		Actor->OnAttributeChanged(AttributeChange::Health, GetHealth(), GetMaxHealth(), Data.EvaluatedData.Magnitude);
	}
}

void UMyAttributeSet::PostMaxHealthEffectExecutor(const FGameplayEffectModCallbackData& Data)
{
	const auto Actor = Cast<ACharacterBase>(GetOwningActor());
	if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Override) // Currently just think that it is happened due to level up.
	{
		Actor->OnAttributeChanged(AttributeChange::Health, GetHealth(), GetMaxHealth(), Data.EvaluatedData.Magnitude);
	}
}

void UMyAttributeSet::PostManaEffectExecutor(const FGameplayEffectModCallbackData& Data)
{
	const auto Actor = Cast<ACharacterBase>(GetOwningActor());
	if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		const float FCurMana = GetMana();
		const float FMaxMana = GetMaxMana();
		Actor->OnAttributeChanged(AttributeChange::Mana, FCurMana, FMaxMana, Data.EvaluatedData.Magnitude);
		UE_LOG(LogTemp, Warning, TEXT("PostManaEffectExecutor %f - %f: %f"), FCurMana, FMaxMana, Data.EvaluatedData.Magnitude);
	}
	else if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Override) // Currently just think that it is happened due to level up.
	{
		Actor->OnAttributeChanged(AttributeChange::Mana, GetMana(), GetMaxMana(), Data.EvaluatedData.Magnitude);
	}
}

void UMyAttributeSet::PostXpEffectExecutor(const FGameplayEffectModCallbackData& Data)
{
	const auto Actor = Cast<ACharacterBase>(GetOwningActor());
	if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		const float FCurXp = GetXp();
		const float FMaxXp = GetMaxXp();
		Actor->OnAttributeChanged(AttributeChange::Xp, FCurXp, FMaxXp, Data.EvaluatedData.Magnitude);
		if (FMath::IsNearlyEqual(FCurXp, FMaxXp) && GetLevel() < GetMaxLevel() && Actor->bAllowLevelUp)
		{
			// Don't allow to level up, until the effects are applied and reset xp to 0.
			Actor->bAllowLevelUp = false;
			Actor->OnNextLevel();
		}
		UE_LOG(LogTemp, Warning, TEXT("PostXpEffectExecutor %f - %f: %f"), FCurXp, FMaxXp, Data.EvaluatedData.Magnitude);
	}
	else if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Override)
	{
		const float FCurXp = GetXp();
		Actor->OnAttributeChanged(AttributeChange::Xp, FCurXp, GetMaxXp(), Data.EvaluatedData.Magnitude);
		
		// Means we on the next level and reset the xp.
		if (FMath::IsNearlyEqual(FCurXp, 0))
		{
			Actor->bAllowLevelUp = true;
		}
	}
}

void UMyAttributeSet::PostSpeedEffectExecutor(const FGameplayEffectModCallbackData& Data)
{
	const auto Actor = Cast<ACharacterBase>(GetOwningActor());
	Actor->GetCharacterMovement()->MaxWalkSpeed = GetSpeed();
	Actor->GetCharacterMovement()->Velocity.Normalize(1);
	Actor->GetCharacterMovement()->Velocity *= GetSpeed();
	UE_LOG(LogTemp, Warning, TEXT("PostSpeedEffectExecutor %f - %f"), GetSpeed(), Speed.GetBaseValue());
}

void UMyAttributeSet::PostMaxSpeedEffectExecutor(const FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("PostMaxSpeedEffectExecutor %f"), GetMaxSpeed());
}

void UMyAttributeSet::PostLevelEffectExecutor(const FGameplayEffectModCallbackData& Data)
{
	const auto Actor = Cast<ACharacterBase>(GetOwningActor());
	if (Actor && Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		const float FCurLevel = GetLevel();
		const float FMaxLevel = GetMaxLevel();
		Actor->OnAttributeChanged(AttributeChange::Level, FCurLevel, FMaxLevel, Data.EvaluatedData.Magnitude);
		UE_LOG(LogTemp, Warning, TEXT("PostLevelffectExecutor %f - %f: %f"), FCurLevel, FMaxLevel, Data.EvaluatedData.Magnitude);
	}
}
