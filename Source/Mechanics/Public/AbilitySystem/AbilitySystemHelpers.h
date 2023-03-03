#pragma once

class UAbilitySystemComponent;
class UGameplayAbilityWithInput;
class UGameplayAbility;
class IAbilitySystemInterface;
class UGameplayEffect;
struct FGameplayAbilitySpec;

class NO_API AbilitySystemHelpers final
{
	AbilitySystemHelpers();
	~AbilitySystemHelpers();

public:
	static void ApplyGameplayEffects(UAbilitySystemComponent* AbilitySystemComponent,
		const TArray<TSubclassOf<UGameplayEffect>>& GameplayEffectClasses, const float Level);

	static void GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent,
	const TArray<TSubclassOf<UGameplayAbilityWithInput>>& AbilityClasses, const int32 Level);

	static FGameplayAbilitySpec GrantAbility(UAbilitySystemComponent* AbilitySystemComponent,
		const TSubclassOf<UGameplayAbilityWithInput>& AbilityClass, const int32 Level);
	
	/**
	 * You must use UGameplayAbilityWithInput class for your ability system to be able to bind input by this method.
	 */
	static void BindInputToAbilities(UAbilitySystemComponent* AbilitySystemComponent);
};
