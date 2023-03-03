#pragma once
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityWithInput.generated.h"

UENUM(BlueprintType)
enum class MyAbilityInputID: uint8
{
	None,
	Confirm,
	Cancel,
	MainAction, // Left mouse button for PC
	SecondAction, // Right mouse button for PC
	ChangeMainAbility,
	ChangeSecondAbility
};

UENUM(BlueprintType)
enum class MyAbilityTypes: uint8
{
	Main, // Left mouse button for PC
	Secondary // Right mouse button for PC
};

/** Standard abilities with hardcode activation input code. */
UCLASS(Blueprintable)
class NO_API UGameplayAbilityWithInput : public UGameplayAbility
{
	GENERATED_BODY()

public:

	// This input id is intended for activating the ability
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	MyAbilityInputID AbilityInputID = MyAbilityInputID::None;

	/** Icon to display */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UTexture* AbilityHUDIcon;
	
	int32 getInputId() const
	{
		return static_cast<int32>(AbilityInputID);
	}
};
