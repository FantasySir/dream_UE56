// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/TimePauseInterface.h"
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
class DREAM_UE56_API AEWCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public ITimePauseInterface
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

	/** TimePause Interface */
	virtual FOnTimePauseImmuneChanged& GetOnTimePauseImmuneChanged() override { return OnTimePauseImmuneChanged; }
	virtual bool IsImmuneToTimePause_Implementation() const override{ return bImmuneTimePause; };
	virtual void OnTimePaused_Implementation() override;
	virtual void OnTimeResumed_Implementation() override;
	virtual void OnTimePauseImmuneStatusChanged_Implementation(bool bIsImmune) override;
	virtual void OnBeforeDestroy_Implementation() override;
	/** end TimePause Interface */

	FOnTimePauseImmuneChanged OnTimePauseImmuneChanged;

	//是否被沉默
	bool bIsSilenced = false;

	//是否被眩晕
	bool bIsStunned = false;

	// 设置时间暂停免疫状态
	UFUNCTION(BlueprintCallable, Category = "Time Pause")
	void SetTimePauseImmune(bool bImmune);

	// 获取时间暂停免疫状态
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Time Pause")
	bool GetTimePauseImmune() const { return bImmuneTimePause; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UEWBaseAttributeSet> BaseAttributeSet;

	//是否免疫暂停时间
	bool bImmuneTimePause = false;

	void SetupPerceptionStimuli();
	virtual void InitAbilityActorInfo();

	// 重写BeginDestroy以在销毁前清理时间管理器引用
	virtual void BeginDestroy() override;

	// 注册到时间管理器
	void TimeManagerRegistered();
	
	//是否死亡
	bool bDead = false;
};
