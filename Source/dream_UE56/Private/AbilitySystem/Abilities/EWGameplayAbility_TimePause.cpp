// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/EWGameplayAbility_TimePause.h"
#include "Character/EWCharacterBase.h"
#include "Gameplay/EWTimeManager.h"
#include "Core/EWGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Engine/World.h"
#include "TimerManager.h"

UEWGameplayAbility_TimePause::UEWGameplayAbility_TimePause()
{
	// 设置能力的基础属性
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(FEWGameplayTags::Ability_TimePause);
	SetAssetTags(AssetTags);
	
	// 设置激活所需的标签
	ActivationRequiredTags.AddTag(FEWGameplayTags::State_Alive);
	
	// 设置阻止激活的标签
	ActivationBlockedTags.AddTag(FEWGameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(FEWGameplayTags::State_Stunned);

	// 设置实例化策略
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 设置网络执行策略
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 设置默认值
	ActionPointCost = 20.0f;
	CooldownDuration = 5.0f;
	PauseDuration = 0.0f; // 0表示手动控制
}

void UEWGameplayAbility_TimePause::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 获取角色
	AEWCharacterBase* Character = Cast<AEWCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 消耗行动点
	if (ActionPointCostEffect && GetAbilitySystemComponentFromActorInfo())
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(ActionPointCostEffect, GetAbilityLevel());
		if (SpecHandle.IsValid())
		{
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	// 激活时间暂停
	if (UEWTimeManager* TimeManager = Character->GetTimeManager())
	{
		if (TimeManager->IsTimePaused())
		{
			// 如果时间已经暂停，则恢复时间
			TimeManager->ResumeTime();
		}
		else
		{
			// 暂停时间
			TimeManager->PauseTime(Character);
			
			// 如果设置了自动恢复时间，启动计时器
			if (PauseDuration > 0.0f)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					World->GetTimerManager().SetTimer(PauseTimerHandle, this, &UEWGameplayAbility_TimePause::OnPauseTimeExpired, PauseDuration, false);
				}
			}
		}
	}

	// 结束能力
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UEWGameplayAbility_TimePause::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 清除计时器
	if (PauseTimerHandle.IsValid())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(PauseTimerHandle);
		}
		PauseTimerHandle.Invalidate();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UEWGameplayAbility_TimePause::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// 检查角色是否能够暂停时间
	AEWCharacterBase* Character = Cast<AEWCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return false;
	}

	return Character->CanPauseTime();
}

void UEWGameplayAbility_TimePause::OnPauseTimeExpired()
{
	// 自动恢复时间
	if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
	{
		AEWCharacterBase* Character = Cast<AEWCharacterBase>(ActorInfo->AvatarActor.Get());
		if (Character)
		{
			if (UEWTimeManager* TimeManager = Character->GetTimeManager())
			{
				TimeManager->ResumeTime();
			}
		}
	}
}
