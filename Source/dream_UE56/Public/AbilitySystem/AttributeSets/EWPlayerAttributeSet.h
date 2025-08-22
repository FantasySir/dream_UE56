// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EWPlayerAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 玩家专属属性集，包含行动值
 */
UCLASS()
class DREAM_UE56_API UEWPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEWPlayerAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 基础属性 - 行动值（仅主角使用）
	UPROPERTY(BlueprintReadOnly, Category = "ActionPoint", ReplicatedUsing = OnRep_ActionPoint)
	FGameplayAttributeData ActionPoint;
	ATTRIBUTE_ACCESSORS(UEWPlayerAttributeSet, ActionPoint)

	UPROPERTY(BlueprintReadOnly, Category = "ActionPoint", ReplicatedUsing = OnRep_MaxActionPoint)
	FGameplayAttributeData MaxActionPoint;
	ATTRIBUTE_ACCESSORS(UEWPlayerAttributeSet, MaxActionPoint)

protected:

	// 属性变化回调函数
	UFUNCTION()
	virtual void OnRep_ActionPoint(const FGameplayAttributeData& OldActionPoint);

	UFUNCTION()
	virtual void OnRep_MaxActionPoint(const FGameplayAttributeData& OldMaxActionPoint);
};
