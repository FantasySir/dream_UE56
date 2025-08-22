// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/EWBaseState.h"
#include "Character/EWUnitBase.h"
#include "EWUnitState.generated.h"

class UEWUnitAttributeSet;
class UEWUnitData;

/**
 * 单位状态类
 * 管理单位的持久化数据，包括装备、技能、AI配置等
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API UEWUnitState : public UEWBaseState
{
	GENERATED_BODY()

public:
	UEWUnitState();

	// 重写初始化
	virtual void InitializeState() override;

	// 获取单位专属属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	UEWUnitAttributeSet* GetUnitAttributeSet() const { return UnitAttributeSet; }

	// 单位基础信息
	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	FString GetUnitName() const { return UnitName; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	void SetUnitName(const FString& Name) { UnitName = Name; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	EUnitFaction GetFaction() const { return Faction; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	void SetFaction(EUnitFaction InFaction) { Faction = InFaction; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	EUnitClass GetUnitClass() const { return UnitClass; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	void SetUnitClass(EUnitClass InClass) { UnitClass = InClass; }

	UFUNCTION(BlueprintCallable, Category = "Unit Info")
	int32 GetUnitLevel() const { return UnitLevel; }

	// 经验值和等级
	UFUNCTION(BlueprintCallable, Category = "Progression")
	void AddExperience(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Progression")
	float GetExperience() const { return Experience; }

	UFUNCTION(BlueprintCallable, Category = "Progression")
	float GetExperiencePercentage() const;

	// 战斗相关属性
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetDefense() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetCriticalChance() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetCriticalMultiplier() const;

	// 单位数据配置
	UFUNCTION(BlueprintCallable, Category = "Configuration")
	void SetUnitData(UEWUnitData* InUnitData);

	UFUNCTION(BlueprintCallable, Category = "Configuration")
	UEWUnitData* GetUnitData() const { return UnitData; }

	// 召唤消耗计算
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	float GetSummonCost() const;

	UFUNCTION(BlueprintCallable, Category = "Summoning")
	bool CanBeSummoned() const;

	// 复活相关
	UFUNCTION(BlueprintCallable, Category = "Revival")
	void StartRevivalCountdown(float RevivalTime = 30.0f);

	UFUNCTION(BlueprintCallable, Category = "Revival")
	void Revive(float HealthPercentage = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Revival")
	bool IsReviving() const { return bIsReviving; }

	UFUNCTION(BlueprintCallable, Category = "Revival")
	float GetRevivalTimeRemaining() const;

	// 装备系统（预留）
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItem(class UObject* Item, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipItem(int32 SlotIndex);

protected:
	// 单位专属属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UEWUnitAttributeSet> UnitAttributeSet;

	// 单位基础信息
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit Info")
	FString UnitName = TEXT("Unknown Unit");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit Info")
	EUnitFaction Faction = EUnitFaction::Neutral;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit Info")
	EUnitClass UnitClass = EUnitClass::None;

	UPROPERTY(BlueprintReadOnly, Category = "Progression")
	int32 UnitLevel = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Progression")
	float Experience = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Progression")
	float ExperiencePerLevel = 50.0f;

	// 单位数据配置
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Configuration")
	TObjectPtr<UEWUnitData> UnitData;

	// 召唤相关
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Summoning")
	float BaseSummonCost = 30.0f;

	// 复活相关
	UPROPERTY(BlueprintReadOnly, Category = "Revival")
	bool bIsReviving = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Revival")
	float DefaultRevivalTime = 30.0f;

	UPROPERTY()
	FTimerHandle RevivalTimerHandle;

	// 装备槽位（预留）
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	TArray<class UObject*> EquippedItems;

public:
	// 重写Character生成
	virtual AActor* SpawnCharacter(UWorld* World, const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator) override;

	// 重写同步方法
	virtual void SyncStateToCharacter() override;
	virtual void SyncStateFromCharacter() override;

	// 重写属性变化回调
	virtual void OnAttributeChangedInternal(const FOnAttributeChangeData& Data) override;

private:
	// 计算等级
	void UpdateLevel();

	// 复活完成回调
	UFUNCTION()
	void OnRevivalComplete();

	// 应用单位数据
	void ApplyUnitData();
};
