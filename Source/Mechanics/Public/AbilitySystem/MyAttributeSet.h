// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "MyAttributeSet.generated.h"

/**
 * This defines a set of helper functions for accessing and initializing attributes, to avoid having to manually write these functions.
 * It would creates the following functions, for attribute Health
 *
 *	static FGameplayAttribute UMyHealthSet::GetHealthAttribute();
 *	FORCEINLINE float UMyHealthSet::GetHealth() const;
 *	FORCEINLINE void UMyHealthSet::SetHealth(float NewVal);
 *	FORCEINLINE void UMyHealthSet::InitHealth(float NewVal);
 *
 * To use this in your game you can define something like this, and then add game-specific functions as necessary:
 *
 *	#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
 *
 *	ATTRIBUTE_ACCESSORS(UMyHealthSet, Health)
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UENUM(BlueprintType)
enum class AttributeChange: uint8
{
	Health,
	Stamina,
	Xp,
	Mana,
	Level
};


UCLASS()
class MECHANICS_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UMyAttributeSet();
	
	// Returns the properties used for network replication.
	// Unreal will replicate properties, structs, and references to other objects automatically over the network.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// In Case if we init attributes from Data Table
	virtual void InitFromMetaDataTable(const UDataTable* DataTable) override;
	
	// Character Attributes (Characteristics) //////////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Mana);
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldManna);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxMana);
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Level);
	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxLevel)
	FGameplayAttributeData MaxLevel;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxLevel);
	UFUNCTION()
	virtual void OnRep_MaxLevel(const FGameplayAttributeData& OldMaxLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Xp)
	FGameplayAttributeData Xp;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Xp);
	UFUNCTION()
	virtual void OnRep_Xp(const FGameplayAttributeData& OldXp);

	// To achieve next level.
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxXp)
	FGameplayAttributeData MaxXp;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxXp);
	UFUNCTION()
	virtual void OnRep_MaxXp(const FGameplayAttributeData& OldMaxXp);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Stamina);
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxStamina);
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Strength);
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Speed);
	UFUNCTION()
	virtual void OnRep_Speed(const FGameplayAttributeData& OldSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxSpeed)
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxSpeed);
	UFUNCTION()
	virtual void OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_FightSkill)
	FGameplayAttributeData FightSkill;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, FightSkill);
	UFUNCTION()
	virtual void OnRep_FightSkill(const FGameplayAttributeData& OldFightSkill);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Basic workflow functions. #0 Execute_Implementation method in case if GameplayEffectCalculation is implemented.
	// #1
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data) override;

	// #2
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// #3
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// #4
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data) override;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	virtual void PostStaminaEffectExecutor(const FGameplayEffectModCallbackData& Data);
	
	virtual void PostHealthEffectExecutor(const FGameplayEffectModCallbackData& Data);

	virtual void PostMaxHealthEffectExecutor(const FGameplayEffectModCallbackData& Data);

	virtual void PostManaEffectExecutor(const FGameplayEffectModCallbackData& Data);

	virtual void PostXpEffectExecutor(const FGameplayEffectModCallbackData& Data);

	virtual void PostSpeedEffectExecutor(const FGameplayEffectModCallbackData& Data);

	virtual void PostMaxSpeedEffectExecutor(const FGameplayEffectModCallbackData& Data);

	virtual void PostLevelEffectExecutor(const FGameplayEffectModCallbackData& Data);
};
