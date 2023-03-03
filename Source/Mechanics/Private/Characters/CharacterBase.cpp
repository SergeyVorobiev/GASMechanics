// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Looks like this is invoked after each PreAttributeChange instead of PostGameplayEffect and it is not invoked after initializing attributes
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetStaminaAttribute()).AddUObject(this, &ACharacterBase::OnStaminaChangedInternal);
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Buff.SpeedStun")),
	                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &ACharacterBase::SpeedStunTagChanged);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ACharacterBase::EffectRemoved);
	Attributes = CreateDefaultSubobject<UMyAttributeSet>("Attributes");
}

void ACharacterBase::SpeedStunTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bSpeedStun = NewCount;
	UE_LOG(LogTemp, Warning, TEXT("TagStunCount: %d"), bSpeedStun);
}

void ACharacterBase::EffectRemoved(const FActiveGameplayEffect& GameplayEffect)
{
	UE_LOG(LogTemp, Warning, TEXT("EffectRemoved: %s"), TEXT("Hello"));
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FGameplayTagContainer ACharacterBase::GetTags() const
{
	FGameplayTagContainer Container = FGameplayTagContainer();
	AbilitySystemComponent->GetOwnedGameplayTags(Container);
	return Container;
}

UTexture* ACharacterBase::AddNewAbility(const TSubclassOf<UGameplayAbilityWithInput> Ability, const MyAbilityTypes Type)
{
	if (Ability)
	{
		switch (Type)
		{
		case MyAbilityTypes::Main:
			MainCircularAbilities.Add(Ability);
			return Ability.GetDefaultObject()->AbilityHUDIcon;
		case MyAbilityTypes::Secondary:
			SecondCircularAbilities.Add(Ability);
			return Ability.GetDefaultObject()->AbilityHUDIcon;
		default:
			return nullptr;
		}
	}
	return nullptr;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	AbilitySystemHelpers::BindInputToAbilities(AbilitySystemComponent);
}

// Only called on the server and when the pawn is possessed by a controller
void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!bAbilitiesInitialized)
	{
		InitAbilities();
	}
}

void ACharacterBase::InitAbilities()
{
	bAbilitiesInitialized = true;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemHelpers::GiveAbilities(AbilitySystemComponent, Abilities, Attributes->GetLevel());
	AbilitySystemHelpers::ApplyGameplayEffects(AbilitySystemComponent, GameplayEffects, Attributes->GetLevel());
	this->GrantNextMainAbility();
	this->GrantNextSecondAbility();
}

// Use this as a subscriber to GetGameplayAttributeValueChangeDelegate if need.
void ACharacterBase::OnStaminaChangedInternal(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("OnStaminaChangedInternal"));
}

void ACharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (!bAbilitiesInitialized)
	{
		InitAbilities();
	}
}

const UGameplayAbility* ACharacterBase::GrantNextMainAbility()
{
	GrantNextCircularAbility(MainCircularAbilities, MainCurrentCircularAbilityIndex, MainCurrentCircularAbilitySpec);
	OnAbilityHUDIconHasBeenChanged(GetHUDIconFromCircularAbility(MainCurrentCircularAbilitySpec), MyAbilityTypes::Main);
	return MainCurrentCircularAbilitySpec.Ability;
}

const UGameplayAbility* ACharacterBase::GrantNextSecondAbility()
{
	GrantNextCircularAbility(SecondCircularAbilities, SecondCurrentCircularAbilityIndex,
	                         SecondCurrentCircularAbilitySpec);
	OnAbilityHUDIconHasBeenChanged(GetHUDIconFromCircularAbility(SecondCurrentCircularAbilitySpec),
	                               MyAbilityTypes::Secondary);
	return SecondCurrentCircularAbilitySpec.Ability;
}

bool ACharacterBase::CanMove() const
{
	return bSpeedStun == 0;
}

const UTexture* ACharacterBase::GetHUDIconFromCircularAbility(
	const FGameplayAbilitySpec& CurrentCircularAbilitySpec) const
{
	return MainCurrentCircularAbilitySpec.Ability
		       ? Cast<UGameplayAbilityWithInput>(CurrentCircularAbilitySpec.Ability)->AbilityHUDIcon
		       : nullptr;
}

void ACharacterBase::GrantNextCircularAbility(
	const TArray<TSubclassOf<UGameplayAbilityWithInput>>& CircularAbilities,
	int32& CurrentCircularAbilityIndex, FGameplayAbilitySpec& CurrentCircularAbilitySpec) const
{
	const auto size = CircularAbilities.Num();
	const auto prevAbIndex = CurrentCircularAbilityIndex;
	if (size > 0) // Have abilities to add
	{
		if (++CurrentCircularAbilityIndex >= size) // Meet the end start from 0
		{
			CurrentCircularAbilityIndex = 0;
		}
		const auto CircularAbility = CircularAbilities[CurrentCircularAbilityIndex];
		const auto NewAbilitySpec = AbilitySystemHelpers::GrantAbility(AbilitySystemComponent, CircularAbility,
		                                                               Attributes->GetLevel());
		const auto NewAbilitySpecHandle = NewAbilitySpec.Handle;
		if (NewAbilitySpecHandle.IsValid())
		{
			AbilitySystemComponent->ClearAbility(CurrentCircularAbilitySpec.Handle);
			CurrentCircularAbilitySpec = NewAbilitySpec;
		}
		else
		{
			CurrentCircularAbilityIndex = prevAbIndex;
		}
	}
}
