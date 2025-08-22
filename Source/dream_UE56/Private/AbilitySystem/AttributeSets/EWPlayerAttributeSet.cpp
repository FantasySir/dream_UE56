// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"

UEWPlayerAttributeSet::UEWPlayerAttributeSet()
{
	// 初始化默认属性值
	InitActionPoint(100.0f);
	InitMaxActionPoint(100.0f);
}

void UEWPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 确保当前值不超过最大值且不为负数
	if (Attribute == GetActionPointAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxActionPoint());
	}
	else if (Attribute == GetMaxActionPointAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UEWPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 直接修改行动值时，确保在范围内
	if (Data.EvaluatedData.Attribute == GetActionPointAttribute())
	{
		SetActionPoint(FMath::Clamp(GetActionPoint(), 0.0f, GetMaxActionPoint()));
	}
}

void UEWPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEWPlayerAttributeSet, ActionPoint, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWPlayerAttributeSet, MaxActionPoint, COND_None, REPNOTIFY_Always);
}

void UEWPlayerAttributeSet::OnRep_ActionPoint(const FGameplayAttributeData& OldActionPoint)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWPlayerAttributeSet, ActionPoint, OldActionPoint);
}

void UEWPlayerAttributeSet::OnRep_MaxActionPoint(const FGameplayAttributeData& OldMaxActionPoint)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWPlayerAttributeSet, MaxActionPoint, OldMaxActionPoint);
}
