// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/EWBaseState.h"
#include "EWPlayerState.generated.h"

class UEWPlayerAttributeSet;
class UEWUnitManager;
class UEWUnitState;

/**
 * 玩家状态类
 * 管理玩家主角的持久化数据，包括单位管理、技能解锁等
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API UEWPlayerState : public UEWBaseState
{
	GENERATED_BODY()

public:
	UEWPlayerState();

	// 重写初始化
	virtual void InitializeState() override;

	// 获取玩家专属属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	UEWPlayerAttributeSet* GetPlayerAttributeSet() const { return PlayerAttributeSet; }

	// 获取单位管理器
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	UEWUnitManager* GetUnitManager() const { return UnitManager; }

	// 行动值相关
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetActionPoint() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxActionPoint() const;

	// 单位管理
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void AddOwnedUnit(UEWUnitState* UnitState);

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void RemoveOwnedUnit(UEWUnitState* UnitState);

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	TArray<UEWUnitState*> GetOwnedUnits() const { return OwnedUnits; }

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	TArray<UEWUnitState*> GetActiveUnits() const { return ActiveUnits; }

	// 设置参战单位
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	bool SetActiveUnit(int32 SlotIndex, UEWUnitState* UnitState);

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	UEWUnitState* GetActiveUnitAtSlot(int32 SlotIndex) const;

	// 召唤单位
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	class AEWUnitBase* SummonUnit(int32 SlotIndex, const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator);

	// 收回单位
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void RecallUnit(int32 SlotIndex);

	// 收回所有单位
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void RecallAllUnits();

	// 经验值和等级
	UFUNCTION(BlueprintCallable, Category = "Progression")
	void AddExperience(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Progression")
	float GetExperience() const { return Experience; }

	UFUNCTION(BlueprintCallable, Category = "Progression")
	int32 GetLevel() const { return Level; }

	// 检查是否可以暂停时间
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	bool CanPauseTime() const;

protected:
	// 玩家专属属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UEWPlayerAttributeSet> PlayerAttributeSet;

	// 单位管理器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Management")
	TObjectPtr<UEWUnitManager> UnitManager;

	// 拥有的单位列表
	UPROPERTY(BlueprintReadOnly, Category = "Unit Management")
	TArray<UEWUnitState*> OwnedUnits;

	// 当前参战的单位列表（对应战斗槽位）
	UPROPERTY(BlueprintReadOnly, Category = "Unit Management")
	TArray<UEWUnitState*> ActiveUnits;

	// 经验值和等级
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Progression")
	float Experience = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Progression")
	int32 Level = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Progression")
	float ExperiencePerLevel = 100.0f;

	// 时间暂停相关
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time Control")
	float PauseTimeCost = 20.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time Control")
	float PauseTimeCooldown = 5.0f;

	UPROPERTY()
	float LastPauseTime = 0.0f;

	// 重写Character生成
	virtual AActor* SpawnCharacter(UWorld* World, const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator) override;

	// 重写同步方法
	virtual void SyncStateToCharacter() override;
	virtual void SyncStateFromCharacter() override;

private:
	// 计算等级
	void UpdateLevel();

	// 单位死亡回调
	UFUNCTION()
	void OnUnitDeath(UEWBaseState* DeadUnit);
};
