// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AttributeSets/EWCombatAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"

UEWCombatAttributeSet::UEWCombatAttributeSet()
{
	// 初始化默认属性值
	InitPhysicalAttack(10.0f);
	InitMagicalAttack(10.0f);
	InitPhysicalDefense(5.0f);
	InitMagicalDefense(5.0f);
	InitMovementSpeed(600.0f);
}

void UEWCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 确保属性不为负数
	if (Attribute == GetPhysicalAttackAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetMagicalAttackAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetPhysicalDefenseAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetMagicalDefenseAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UEWCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 确保所有属性在合理范围内
	if (Data.EvaluatedData.Attribute == GetPhysicalAttackAttribute())
	{
		SetPhysicalAttack(FMath::Max(GetPhysicalAttack(), 0.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetMagicalAttackAttribute())
	{
		SetMagicalAttack(FMath::Max(GetMagicalAttack(), 0.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetPhysicalDefenseAttribute())
	{
		SetPhysicalDefense(FMath::Max(GetPhysicalDefense(), 0.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetMagicalDefenseAttribute())
	{
		SetMagicalDefense(FMath::Max(GetMagicalDefense(), 0.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetMovementSpeedAttribute())
	{
		SetMovementSpeed(FMath::Max(GetMovementSpeed(), 0.0f));
	}
}

void UEWCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEWCombatAttributeSet, PhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWCombatAttributeSet, MagicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWCombatAttributeSet, PhysicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWCombatAttributeSet, MagicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWCombatAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
}

void UEWCombatAttributeSet::OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWCombatAttributeSet, PhysicalAttack, OldPhysicalAttack);
}

void UEWCombatAttributeSet::OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWCombatAttributeSet, MagicalAttack, OldMagicalAttack);
}

void UEWCombatAttributeSet::OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWCombatAttributeSet, PhysicalDefense, OldPhysicalDefense);
}

void UEWCombatAttributeSet::OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWCombatAttributeSet, MagicalDefense, OldMagicalDefense);
}

void UEWCombatAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWCombatAttributeSet, MovementSpeed, OldMovementSpeed);
}
