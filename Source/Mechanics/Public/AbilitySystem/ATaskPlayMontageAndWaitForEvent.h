// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ATaskPlayMontageAndWaitForEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventReceivedDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

// Looks like we can not just extend from PlayMontageAndWait to avoid copying this whole crap.
/**
 * 
 */
UCLASS()
class MECHANICS_API UATaskPlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	UATaskPlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate	OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate	OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate EventReceived;
	
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) const;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnAbilityCancelled();
	
	virtual void Activate() override;

	virtual void ExternalCancel() override;

	bool StopPlayingMontage() const;
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UATaskPlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility,
		UAnimMontage* MontageToPlay, const FGameplayTagContainer EventTags, float Rate, const FName StartSection);

protected:
	
	UPROPERTY()
	UAnimMontage* MontageToPlay;
	
	UPROPERTY()
	FGameplayTagContainer EventTags;
	
	UPROPERTY()
	float Rate;
	
	UPROPERTY()
	FName StartSection;
	
	FOnMontageBlendingOutStarted BlendingOutDelegate;

	FOnMontageEnded MontageEndedDelegate;

	FDelegateHandle CancelledHandleDelegate;

	FDelegateHandle EventHandleDelegate;

	virtual void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) const;

	virtual void OnDestroy(bool AbilityEnded) override;
};
