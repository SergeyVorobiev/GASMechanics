// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/ATaskPlayMontageAndWaitForEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"

UATaskPlayMontageAndWaitForEvent::UATaskPlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Rate = 1.f;
}

void UATaskPlayMontageAndWaitForEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}
	bool bPlayedMontage = false;
	if (AbilitySystemComponent != nullptr)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			EventHandleDelegate = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags,
				FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UATaskPlayMontageAndWaitForEvent::OnGameplayEvent));
			if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
			{
				// Looks like extra check or it should be on start?
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}
				
				CancelledHandleDelegate = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UATaskPlayMontageAndWaitForEvent::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UATaskPlayMontageAndWaitForEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UATaskPlayMontageAndWaitForEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character && (Character->GetLocalRole() == ROLE_Authority ||
								  (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy()
								  	== EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(1);
				}
				bPlayedMontage = true;
			}
		}
	}
	if (!bPlayedMontage) {
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	SetWaitingOnAvatar();
}

void UATaskPlayMontageAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandleDelegate);
	}
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandleDelegate);
	}
	Super::OnDestroy(AbilityEnded);
}

void UATaskPlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) const
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;
		EventReceived.Broadcast(EventTag, TempData);
	}
}

UATaskPlayMontageAndWaitForEvent* UATaskPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility,
	UAnimMontage* MontageToPlay, const FGameplayTagContainer EventTags, float Rate, const FName StartSection)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);
	UATaskPlayMontageAndWaitForEvent* Task = NewAbilityTask<UATaskPlayMontageAndWaitForEvent>(OwningAbility);
	Task->MontageToPlay = MontageToPlay;
	Task->EventTags = EventTags;
	Task->Rate = Rate;
	Task->StartSection = StartSection;
	return Task;
}

void UATaskPlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) const
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);
			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character && (Character->GetLocalRole() == ROLE_Authority ||
							  (Character->GetLocalRole() == ROLE_AutonomousProxy &&
							  	Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(1);
			}
		}
	}
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (bInterrupted)
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		} else
		{
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UATaskPlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	EndTask();
}

void UATaskPlayMontageAndWaitForEvent::ExternalCancel()
{
	OnAbilityCancelled();
	Super::ExternalCancel();
}

void UATaskPlayMontageAndWaitForEvent::OnAbilityCancelled()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

bool UATaskPlayMontageAndWaitForEvent::StopPlayingMontage() const
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}
	const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}
	if (AbilitySystemComponent != nullptr && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability
			&& AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}
			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}
	return false;
}
