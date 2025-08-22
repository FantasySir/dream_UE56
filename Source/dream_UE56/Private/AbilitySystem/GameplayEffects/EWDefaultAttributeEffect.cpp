// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/GameplayEffects/EWDefaultAttributeEffect.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWCombatAttributeSet.h"

UEWDefaultAttributeEffect::UEWDefaultAttributeEffect()
{
	// 设置效果类型为瞬时效果
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 添加修改器来设置最大血量
	FGameplayModifierInfo MaxHealthModifier;
	MaxHealthModifier.Attribute = UEWBaseAttributeSet::GetMaxHealthAttribute();
	MaxHealthModifier.ModifierOp = EGameplayModOp::Override;
	MaxHealthModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.0f));
	Modifiers.Add(MaxHealthModifier);

	// 添加修改器来设置当前血量
	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = UEWBaseAttributeSet::GetHealthAttribute();
	HealthModifier.ModifierOp = EGameplayModOp::Override;
	HealthModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.0f));
	Modifiers.Add(HealthModifier);

	// 添加魔法值设置
	FGameplayModifierInfo MaxManaModifier;
	MaxManaModifier.Attribute = UEWBaseAttributeSet::GetMaxManaAttribute();
	MaxManaModifier.ModifierOp = EGameplayModOp::Override;
	MaxManaModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.0f));
	Modifiers.Add(MaxManaModifier);

	FGameplayModifierInfo ManaModifier;
	ManaModifier.Attribute = UEWBaseAttributeSet::GetManaAttribute();
	ManaModifier.ModifierOp = EGameplayModOp::Override;
	ManaModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.0f));
	Modifiers.Add(ManaModifier);

	// 添加行动值设置（仅适用于玩家）
	FGameplayModifierInfo MaxActionPointModifier;
	MaxActionPointModifier.Attribute = UEWPlayerAttributeSet::GetMaxActionPointAttribute();
	MaxActionPointModifier.ModifierOp = EGameplayModOp::Override;
	MaxActionPointModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.0f));
	Modifiers.Add(MaxActionPointModifier);

	FGameplayModifierInfo ActionPointModifier;
	ActionPointModifier.Attribute = UEWPlayerAttributeSet::GetActionPointAttribute();
	ActionPointModifier.ModifierOp = EGameplayModOp::Override;
	ActionPointModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.0f));
	Modifiers.Add(ActionPointModifier);

	// 添加战斗属性设置
	FGameplayModifierInfo PhysicalAttackModifier;
	PhysicalAttackModifier.Attribute = UEWCombatAttributeSet::GetPhysicalAttackAttribute();
	PhysicalAttackModifier.ModifierOp = EGameplayModOp::Override;
	PhysicalAttackModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(10.0f));
	Modifiers.Add(PhysicalAttackModifier);

	FGameplayModifierInfo MagicalAttackModifier;
	MagicalAttackModifier.Attribute = UEWCombatAttributeSet::GetMagicalAttackAttribute();
	MagicalAttackModifier.ModifierOp = EGameplayModOp::Override;
	MagicalAttackModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(10.0f));
	Modifiers.Add(MagicalAttackModifier);

	FGameplayModifierInfo PhysicalDefenseModifier;
	PhysicalDefenseModifier.Attribute = UEWCombatAttributeSet::GetPhysicalDefenseAttribute();
	PhysicalDefenseModifier.ModifierOp = EGameplayModOp::Override;
	PhysicalDefenseModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(5.0f));
	Modifiers.Add(PhysicalDefenseModifier);

	FGameplayModifierInfo MagicalDefenseModifier;
	MagicalDefenseModifier.Attribute = UEWCombatAttributeSet::GetMagicalDefenseAttribute();
	MagicalDefenseModifier.ModifierOp = EGameplayModOp::Override;
	MagicalDefenseModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(5.0f));
	Modifiers.Add(MagicalDefenseModifier);

	FGameplayModifierInfo MovementSpeedModifier;
	MovementSpeedModifier.Attribute = UEWCombatAttributeSet::GetMovementSpeedAttribute();
	MovementSpeedModifier.ModifierOp = EGameplayModOp::Override;
	MovementSpeedModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(600.0f));
	Modifiers.Add(MovementSpeedModifier);
}
