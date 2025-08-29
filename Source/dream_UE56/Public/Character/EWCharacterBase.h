// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/CombatInterface.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "EWCharacterBase.generated.h"

class UAbilitySystemComponent;
class UEWBaseAttributeSet;
class UEWPlayerAttributeSet;
class UEWCombatAttributeSet;
class AEWUnitBase;
class UGameplayAbility;
class UGameplayEffect;

// 单位状态枚举


UCLASS(Abstract)
class DREAM_UE56_API AEWCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEWCharacterBase();
	//virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//ASC
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UEWBaseAttributeSet* GetBaseAttributeSet() const { return BaseAttributeSet;}
	/** Combat Interface */
	virtual FOnDeathSignature& GetOnDeathDelegate() override { return OnDeathDelegate; }
	virtual FOnDamageSignature& GetOnDamageSignature() override { return OnDamageDelegate; }
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override { return OnAscRegistered; }
	virtual bool IsDead_Implementation() const override;

	/** end Combat Interface */

	FOnASCRegistered OnAscRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;

	//是否被沉默
	bool bIsSilenced = false;

	//是否被眩晕
	bool bIsStunned = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UEWBaseAttributeSet> BaseAttributeSet;


	void SetupPerceptionStimuli();
	virtual void InitAbilityActorInfo();
	
	//是否死亡
	bool bDead = false;	
};
