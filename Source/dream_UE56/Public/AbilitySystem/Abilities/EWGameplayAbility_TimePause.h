// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EWGameplayAbility_TimePause.generated.h"

class AEWCharacterBase;

/**
 * 时间暂停能力
 * 允许玩家暂停游戏时间，消耗行动点
 */
UCLASS()
class DREAM_UE56_API UEWGameplayAbility_TimePause : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEWGameplayAbility_TimePause();

protected:
	// 能力激活
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 能力结束
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// 检查能力是否可以激活
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	// 行动点消耗
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cost")
	float ActionPointCost = 20.0f;

	// 冷却时间
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	float CooldownDuration = 5.0f;

	// 时间暂停持续时间（0表示手动控制）
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time Control")
	float PauseDuration = 0.0f;

	// 消耗行动点的GameplayEffect
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TSubclassOf<class UGameplayEffect> ActionPointCostEffect;

private:
	// 时间暂停计时器句柄
	FTimerHandle PauseTimerHandle;

	// 暂停时间（用于自动恢复）
	UFUNCTION()
	void OnPauseTimeExpired();
};
