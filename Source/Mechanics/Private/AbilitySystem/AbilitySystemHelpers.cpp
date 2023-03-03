#include "AbilitySystem/AbilitySystemHelpers.h"

#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "../Public/AbilitySystem/GameplayAbilityWithInput.h"

AbilitySystemHelpers::AbilitySystemHelpers()
{
}

AbilitySystemHelpers::~AbilitySystemHelpers()
{
}


void AbilitySystemHelpers::ApplyGameplayEffects(UAbilitySystemComponent* AbilitySystemComponent,
                                                const TArray<TSubclassOf<UGameplayEffect>>& GameplayEffectClasses,
                                                const float Level)
{
	const AActor* Actor = AbilitySystemComponent->GetOwnerActor();
	if (Actor && Actor->HasAuthority())
	{
		for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : GameplayEffectClasses)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(Actor);
			const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
				GameplayEffectClass, Level, EffectContext);
			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void AbilitySystemHelpers::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent,
                                         const TArray<TSubclassOf<UGameplayAbilityWithInput>>& AbilityClasses,
                                         const int32 Level)
{
	AActor* Actor = AbilitySystemComponent->GetOwnerActor();
	if (Actor && Actor->HasAuthority())
	{
		for (const TSubclassOf<UGameplayAbilityWithInput>& AbilityClass : AbilityClasses)
		{
			UGameplayAbilityWithInput* AbilityWithInput = AbilityClass.GetDefaultObject();
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(AbilityWithInput, Level, AbilityWithInput->getInputId(), Actor));
		}
	}
}

FGameplayAbilitySpec AbilitySystemHelpers::GrantAbility(UAbilitySystemComponent* AbilitySystemComponent,
                                       const TSubclassOf<UGameplayAbilityWithInput>& AbilityClass, const int32 Level)
{
	AActor* Actor = AbilitySystemComponent->GetOwnerActor();
	if (Actor && Actor->HasAuthority())
	{
		UGameplayAbilityWithInput* AbilityWithInput = AbilityClass.GetDefaultObject();
		const auto AbilitySpec = FGameplayAbilitySpec(AbilityWithInput, Level, AbilityWithInput->getInputId(), Actor);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
		return AbilitySpec;
	}
	return FGameplayAbilitySpec();
}

void AbilitySystemHelpers::BindInputToAbilities(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		const AActor* Actor = AbilitySystemComponent->GetOwnerActor();
		if (Actor && Actor->InputComponent)
		{
			const FGameplayAbilityInputBinds Binds("Confirm", "Cancel",
			                                       "MyAbilityInputID", static_cast<int32>(MyAbilityInputID::Confirm),
			                                       static_cast<int32>(MyAbilityInputID::Cancel));
			AbilitySystemComponent->BindAbilityActivationToInputComponent(Actor->InputComponent, Binds);
		}
	}
}
