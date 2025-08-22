// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "EWBaseState.generated.h"

class UAbilitySystemComponent;
class UEWBaseAttributeSet;
class UEWCombatAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
class AEWCharacterBase;
class AEWUnitBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateAttributeChanged, class UEWBaseState*, State, const struct FGameplayAttribute&, Attribute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateCharacterSpawned, class AActor*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateCharacterDestroyed, class UEWBaseState*, State);

/**
 * 基础状态类
 * 管理角色的持久化数据，独立于Character实例的生命周期
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API UEWBaseState : public UObject, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UEWBaseState();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 初始化状态
	UFUNCTION(BlueprintCallable, Category = "State Management")
	virtual void InitializeState();

	// 创建对应的Character实例
	UFUNCTION(BlueprintCallable, Category = "Character Management")
	virtual AActor* SpawnCharacter(UWorld* World, const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator);

	// 销毁Character实例（保留状态数据）
	UFUNCTION(BlueprintCallable, Category = "Character Management")
	virtual void DestroyCharacter();

	// 获取当前的Character实例
	UFUNCTION(BlueprintCallable, Category = "Character Management", BlueprintPure)
	AActor* GetCharacterActor() const { return CharacterActor.Get(); }

	// 检查Character是否存在
	UFUNCTION(BlueprintCallable, Category = "Character Management", BlueprintPure)
	bool HasCharacterActor() const { return CharacterActor.IsValid(); }

	// 获取属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	UEWBaseAttributeSet* GetBaseAttributeSet() const { return BaseAttributeSet; }

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	UEWCombatAttributeSet* GetCombatAttributeSet() const { return CombatAttributeSet; }

	// 属性相关
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	// 状态标签管理
	UFUNCTION(BlueprintCallable, Category = "State Tags")
	void AddStateTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "State Tags")
	void RemoveStateTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "State Tags")
	bool HasStateTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "State Tags")
	FGameplayTagContainer GetStateTags() const { return StateTags; }

	// 设置Character类
	UFUNCTION(BlueprintCallable, Category = "Configuration")
	void SetCharacterClass(TSubclassOf<AActor> InCharacterClass) { CharacterClass = InCharacterClass; }

	UFUNCTION(BlueprintCallable, Category = "Configuration")
	TSubclassOf<AActor> GetCharacterClass() const { return CharacterClass; }

	// 委托事件
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStateAttributeChanged OnAttributeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStateCharacterSpawned OnCharacterSpawned;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStateCharacterDestroyed OnCharacterDestroyed;

protected:
	// 能力系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UEWBaseAttributeSet> BaseAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UEWCombatAttributeSet> CombatAttributeSet;

	// Character类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TSubclassOf<AActor> CharacterClass;

	// 当前的Character实例
	UPROPERTY()
	TWeakObjectPtr<AActor> CharacterActor;

	// 状态标签
	UPROPERTY(BlueprintReadOnly, Category = "State")
	FGameplayTagContainer StateTags;

	// 默认属性效果
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	// 起始能力
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// 初始化能力系统
	virtual void InitializeAbilitySystem();

	// 同步状态到Character
	virtual void SyncStateToCharacter();

	// 从Character同步状态
	virtual void SyncStateFromCharacter();

	// 属性变化回调
	virtual void OnAttributeChangedInternal(const FOnAttributeChangeData& Data);

private:
	// 绑定Character的事件
	void BindCharacterEvents(AActor* Character);

	// 解绑Character的事件
	void UnbindCharacterEvents(AActor* Character);
};
