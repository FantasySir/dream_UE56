// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/LevelExperienceInterface.h"
#include "Character/EWUnitBase.h"
#include "Net/UnrealNetwork.h"
#include "EWUnitState.generated.h"

class UAbilitySystemComponent;
class UEWUnitAttributeSet;
class UEWBaseAttributeSet;

/**
 * 单位状态类
 * 管理单位的持久化数据，专注于数据存储
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API AEWUnitState : public APlayerState, public IAbilitySystemInterface, public ILevelExperienceInterface
{
	GENERATED_BODY()

public:
	AEWUnitState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// AbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ===================== 等级经验接口实现 =====================
	
	// 等级相关
	virtual int32 GetLevel_Implementation() const override;
	virtual void SetLevel_Implementation(int32 InLevel) override;
	virtual void AddToLevel_Implementation(int32 InLevel) override;

	// 经验相关
	virtual int32 GetExperience_Implementation() const override;
	virtual void SetExperience_Implementation(int32 InExperience) override;
	virtual void AddToExperience_Implementation(int32 InExperience) override;

	// 委托获取
	virtual FOnLevelChanged* GetLevelChangedDelegate() override { return &OnLevelChangedDelegate; }
	virtual FOnExperienceChanged* GetExperienceChangedDelegate() override { return &OnExperienceChangedDelegate; }

	// 获取基础属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System", BlueprintPure)
	UEWBaseAttributeSet* GetBaseAttributeSet() const { return BaseAttributeSet; }

	// 获取单位专属属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	UEWUnitAttributeSet* GetUnitAttributeSet() const { return UnitAttributeSet; }

	// 委托声明
	FOnLevelChanged OnLevelChangedDelegate;
	FOnExperienceChanged OnExperienceChangedDelegate;

	// ===================== 基础信息 Get/Set =====================
	
	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	FORCEINLINE FString GetUnitName() const { return UnitName; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	void SetUnitName(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	FORCEINLINE EUnitFaction GetFaction() const { return Faction; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	void SetFaction(EUnitFaction InFaction);

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	FORCEINLINE EUnitClass GetUnitClass() const { return UnitClass; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	void SetUnitClass(EUnitClass InClass);

	// ===================== 召唤相关 Get/Set =====================
	
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	FORCEINLINE float GetBaseSummonCost() const { return BaseSummonCost; }

	UFUNCTION(BlueprintCallable, Category = "Summoning")
	void SetBaseSummonCost(float InCost);

	// ===================== 复活相关 Get/Set =====================
	
	UFUNCTION(BlueprintCallable, Category = "Revival")
	FORCEINLINE bool IsReviving() const { return bIsReviving; }

	UFUNCTION(BlueprintCallable, Category = "Revival")
	void SetReviving(bool bInReviving);

	UFUNCTION(BlueprintCallable, Category = "Revival")
	FORCEINLINE float GetDefaultRevivalTime() const { return DefaultRevivalTime; }

	UFUNCTION(BlueprintCallable, Category = "Revival")
	void SetDefaultRevivalTime(float InTime);

	// ===================== 装备系统 Get/Set =====================
	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	TArray<UObject*> GetEquippedItems() const { return EquippedItems; }

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UObject* GetEquippedItem(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void SetEquippedItem(int32 SlotIndex, UObject* Item);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	int32 GetEquipmentSlotCount() const { return EquippedItems.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void SetEquipmentSlotCount(int32 SlotCount);

protected:
	// 能力系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 单位专基础属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UEWBaseAttributeSet> BaseAttributeSet;

	// 单位专属属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UEWUnitAttributeSet> UnitAttributeSet;

private:
	// ===================== 等级经验数据 =====================
	
	// 等级
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level, Category = "Level System")
	int32 Level = 1;

	// 经验
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Experience, Category = "Level System")
	int32 Experience = 0;

	// 网络复制回调
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_Experience(int32 OldExperience);

	// ===================== 基础信息数据 =====================
	
	UPROPERTY(EditAnywhere, Replicated, Category = "Unit Info")
	FString UnitName = TEXT("Unknown Unit");

	UPROPERTY(EditAnywhere, Replicated, Category = "Unit Info")
	EUnitFaction Faction = EUnitFaction::Neutral;

	UPROPERTY(EditAnywhere, Replicated, Category = "Unit Info")
	EUnitClass UnitClass = EUnitClass::None;

	// ===================== 召唤数据 =====================
	
	UPROPERTY(EditAnywhere, Replicated, Category = "Summoning")
	float BaseSummonCost = 30.0f;

	// ===================== 复活数据 =====================
	
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Revival")
	bool bIsReviving = false;

	UPROPERTY(EditAnywhere, Replicated, Category = "Revival")
	float DefaultRevivalTime = 30.0f;

	// ===================== 装备数据 =====================
	
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Equipment")
	TArray<UObject*> EquippedItems;
};
