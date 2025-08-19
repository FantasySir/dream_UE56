// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EWUnitAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 单位基础属性集，包含所有游戏属性
 */
UCLASS()
class DREAM_UE56_API UEWUnitAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEWUnitAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 基础属性 - 生命值
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, MaxHealth)

	// 基础属性 - 魔法值
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, MaxMana)

	// 基础属性 - 行动值（仅主角使用）
	UPROPERTY(BlueprintReadOnly, Category = "ActionPoint", ReplicatedUsing = OnRep_ActionPoint)
	FGameplayAttributeData ActionPoint;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, ActionPoint)

	UPROPERTY(BlueprintReadOnly, Category = "ActionPoint", ReplicatedUsing = OnRep_MaxActionPoint)
	FGameplayAttributeData MaxActionPoint;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, MaxActionPoint)

	// 基础属性 - 体力值
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, MaxStamina)

	// 战斗属性 - 攻击
	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_PhysicalAttack)
	FGameplayAttributeData PhysicalAttack;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, PhysicalAttack)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_MagicalAttack)
	FGameplayAttributeData MagicalAttack;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, MagicalAttack)

	// 战斗属性 - 防御
	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_PhysicalDefense)
	FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, PhysicalDefense)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_MagicalDefense)
	FGameplayAttributeData MagicalDefense;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, MagicalDefense)

	// 移动属性
	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, MovementSpeed)

	// 等级
	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_UnitLevel)
	FGameplayAttributeData UnitLevel;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, UnitLevel)

	// 临时属性（用于计算伤害等）
	UPROPERTY(BlueprintReadOnly, Category = "Meta")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Meta")
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UEWUnitAttributeSet, Healing)

protected:

	// 属性变化回调函数
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	virtual void OnRep_ActionPoint(const FGameplayAttributeData& OldActionPoint);

	UFUNCTION()
	virtual void OnRep_MaxActionPoint(const FGameplayAttributeData& OldMaxActionPoint);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

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

	UFUNCTION()
	virtual void OnRep_UnitLevel(const FGameplayAttributeData& OldUnitLevel);

	// 限制属性不超过最大值的辅助函数
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
};
