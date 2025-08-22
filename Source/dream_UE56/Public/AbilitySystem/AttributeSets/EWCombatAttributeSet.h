// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EWCombatAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 战斗属性集，包含攻击、防御、移动速度等单位属性
 */
UCLASS()
class DREAM_UE56_API UEWCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEWCombatAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 战斗属性 - 攻击
	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_PhysicalAttack)
	FGameplayAttributeData PhysicalAttack;
	ATTRIBUTE_ACCESSORS(UEWCombatAttributeSet, PhysicalAttack)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_MagicalAttack)
	FGameplayAttributeData MagicalAttack;
	ATTRIBUTE_ACCESSORS(UEWCombatAttributeSet, MagicalAttack)

	// 战斗属性 - 防御
	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_PhysicalDefense)
	FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(UEWCombatAttributeSet, PhysicalDefense)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_MagicalDefense)
	FGameplayAttributeData MagicalDefense;
	ATTRIBUTE_ACCESSORS(UEWCombatAttributeSet, MagicalDefense)

	// 移动属性
	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UEWCombatAttributeSet, MovementSpeed)

protected:

	// 属性变化回调函数
	UFUNCTION()
	virtual void OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack);

	UFUNCTION()
	virtual void OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack);

	UFUNCTION()
	virtual void OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense);

	UFUNCTION()
	virtual void OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense);

	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);
};
