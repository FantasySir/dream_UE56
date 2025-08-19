// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayEffects/EWDefaultAttributeEffect.h"
#include "Character/EWUnitAttributeSet.h"

UEWDefaultAttributeEffect::UEWDefaultAttributeEffect()
{
	// 设置效果类型为瞬时效果
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 添加修改器来设置最大血量
	FGameplayModifierInfo MaxHealthModifier;
	MaxHealthModifier.Attribute = UEWUnitAttributeSet::GetMaxHealthAttribute();
	MaxHealthModifier.ModifierOp = EGameplayModOp::Override;
	MaxHealthModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.0f));
	Modifiers.Add(MaxHealthModifier);

	// 添加修改器来设置当前血量
	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = UEWUnitAttributeSet::GetHealthAttribute();
	HealthModifier.ModifierOp = EGameplayModOp::Override;
	HealthModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.0f));
	Modifiers.Add(HealthModifier);
}
