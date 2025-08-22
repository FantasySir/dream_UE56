// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/GameplayEffects/EWGameplayEffect_ActionPointCost.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"

UEWGameplayEffect_ActionPointCost::UEWGameplayEffect_ActionPointCost()
{
	// 设置持续时间为即时
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 创建修改器来减少行动点
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UEWPlayerAttributeSet::GetActionPointAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	
	// 创建一个标量修改器，值会在蓝图中设置
	FScalableFloat CostMagnitude;
	CostMagnitude.Value = -20.0f; // 默认消耗20点行动值
	
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(CostMagnitude);

	// 添加修改器到效果
	Modifiers.Add(ModifierInfo);
}
