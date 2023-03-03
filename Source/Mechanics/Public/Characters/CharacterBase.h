// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilitySystemHelpers.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/GameplayAbilityWithInput.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UMyAbilitySystemComponent;
class UMyAttributeSet;
class UGameplayAbilityWithInput;

UCLASS()
class MECHANICS_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System", meta = (AllowPrivateAccess = "true"))
	UMyAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UMyAttributeSet* Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities | Hardcode Input")
	TArray<TSubclassOf<UGameplayAbilityWithInput>> Abilities;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities | Circular")
	TArray<TSubclassOf<UGameplayAbilityWithInput>> MainCircularAbilities;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities | Circular")
	TArray<TSubclassOf<UGameplayAbilityWithInput>> SecondCircularAbilities;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities | Effects")
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Abilities | Current")
	FGameplayAbilitySpec MainCurrentCircularAbilitySpec;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Abilities | Current")
	FGameplayAbilitySpec SecondCurrentCircularAbilitySpec;

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Blueprint functions /////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetTags() const;

	UFUNCTION(BlueprintCallable)
	UTexture* AddNewAbility(const TSubclassOf<UGameplayAbilityWithInput> Ability, const MyAbilityTypes Type);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Events ---------------------------------------
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttributeChanged(AttributeChange Attribute, float CurrentValue, float MaxValue, float ChangedValue);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMakeDamage(float Damage);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float CurrentValue, float ChangeValue, bool IsDead);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilityHUDIconHasBeenChanged(const UTexture* Icon, MyAbilityTypes Type);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnNextLevel();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	int32 bAbilitiesInitialized;

	int32 bSpeedStun;
	
	virtual void EffectRemoved(const FActiveGameplayEffect& GameplayEffect);

	virtual void SpeedStunTagChanged(const FGameplayTag Tag, int32 NewCount);
public:

	// Should be a method
	int32 bAllowLevelUp = true;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void InitAbilities();

	virtual void OnStaminaChangedInternal(const FOnAttributeChangeData& Data);
	
	virtual void OnRep_Controller() override;

	virtual void OnRep_PlayerState() override;
	
	/**
	 * Assign next ability in the array if the array ended then start from 0. Old ability will be removed, if new ability
	 * can not be granted then nothing happens.
	 */
	UFUNCTION(BlueprintCallable, Category = AbilitySystem)
	const UGameplayAbility* GrantNextMainAbility();

	UFUNCTION(BlueprintCallable, Category = AbilitySystem)
	const UGameplayAbility* GrantNextSecondAbility();

	UFUNCTION(BlueprintCallable)
	bool CanMove() const;
private:

	// Track current active circular ability
	int32 MainCurrentCircularAbilityIndex = -1;
	int32 SecondCurrentCircularAbilityIndex = -1;

	void GrantNextCircularAbility(const TArray<TSubclassOf<UGameplayAbilityWithInput>>& CircularAbilities,
		int32& CurrentCircularAbilityIndex, FGameplayAbilitySpec& CurrentCircularAbilitySpec) const;

	const UTexture* GetHUDIconFromCircularAbility(const FGameplayAbilitySpec& CurrentCircularAbilitySpec) const;
};
