// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "EWUnitManager.generated.h"

class AEWUnitBase;

// 单位状态枚举
UENUM(BlueprintType)
enum class EUnitSlotState : uint8
{
	Empty			UMETA(DisplayName = "Empty"),			// 空槽位
	Available		UMETA(DisplayName = "Available"),		// 可用（在备选池中）
	InBattle		UMETA(DisplayName = "InBattle"),		// 在战斗中
	Deployed		UMETA(DisplayName = "Deployed")			// 已部署到战斗槽位
};

// 单位槽位数据
USTRUCT(BlueprintType)
struct DREAM_UE56_API FUnitSlotData
{
	GENERATED_BODY()

	// 单位类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	TSubclassOf<AEWUnitBase> UnitClass;

	// 当前血量百分比（0-1）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	float HealthPercentage = 1.0f;

	// 当前魔法值百分比（0-1）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	float ManaPercentage = 1.0f;

	// 当前状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	EUnitSlotState SlotState = EUnitSlotState::Empty;

	// 槽位索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	int32 SlotIndex = -1;

	// 是否已解锁
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	bool bUnlocked = false;

	FUnitSlotData()
	{
		UnitClass = nullptr;
		HealthPercentage = 1.0f;
		ManaPercentage = 1.0f;
		SlotState = EUnitSlotState::Empty;
		SlotIndex = -1;
		bUnlocked = false;
	}
};

// 单位管理器委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitSlotChanged, int32, SlotIndex, const FUnitSlotData&, SlotData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBattleSlotChanged, int32, BattleSlotIndex, const FUnitSlotData&, SlotData);

/**
 * 单位管理器 - 管理玩家拥有的所有单位
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API UEWUnitManager : public UObject
{
	GENERATED_BODY()

public:
	UEWUnitManager();

	// 初始化管理器
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void Initialize(int32 TotalSlots, int32 MaxBattleSlotsCount);

	// 获取配置参数
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	int32 GetTotalSlots() const { return MaxTotalSlots; }

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	int32 GetBattleSlots() const { return MaxBattleSlots; }

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	int32 GetReserveSlots() const { return MaxTotalSlots - MaxBattleSlots; }

	// 单位获取
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	bool AddUnit(TSubclassOf<AEWUnitBase> UnitClass, int32 SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	bool RemoveUnit(int32 SlotIndex);

	// 获取单位数据
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	FUnitSlotData GetUnitSlot(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	TArray<FUnitSlotData> GetAllUnits() const { return UnitSlots; }

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	TArray<FUnitSlotData> GetAvailableUnits() const;

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	TArray<FUnitSlotData> GetBattleUnits() const { return BattleSlots; }

	// 战斗配置管理
	UFUNCTION(BlueprintCallable, Category = "Battle Configuration")
	bool SetBattleSlot(int32 BattleSlotIndex, int32 UnitSlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Battle Configuration")
	bool ClearBattleSlot(int32 BattleSlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Battle Configuration")
	bool SwapBattleSlots(int32 BattleSlotIndex1, int32 BattleSlotIndex2);

	UFUNCTION(BlueprintCallable, Category = "Battle Configuration")
	void ClearAllBattleSlots();

	// 战斗召唤相关
	UFUNCTION(BlueprintCallable, Category = "Battle Summoning")
	float CalculateSummonCost() const;

	UFUNCTION(BlueprintCallable, Category = "Battle Summoning")
	bool CanSummonAll(float AvailableActionPoint) const;

	UFUNCTION(BlueprintCallable, Category = "Battle Summoning")
	TArray<AEWUnitBase*> SummonBattleUnits(UWorld* World, const TArray<FVector>& SpawnLocations, AActor* Owner);

	// 获取已召唤的单位（蓝图可用）
	UFUNCTION(BlueprintCallable, Category = "Battle Summoning")
	TArray<AEWUnitBase*> GetSummonedUnits() const;

	// 单位状态更新
	UFUNCTION(BlueprintCallable, Category = "Unit Status")
	void UpdateUnitStatus(int32 SlotIndex, float HealthPercentage, float ManaPercentage);

	UFUNCTION(BlueprintCallable, Category = "Unit Status")
	void UpdateUnitFromActor(int32 SlotIndex, AEWUnitBase* UnitActor);

	// 槽位解锁
	UFUNCTION(BlueprintCallable, Category = "Unit Unlock")
	bool UnlockSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Unit Unlock")
	int32 GetUnlockedSlotCount() const;

	// 保存/加载
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveToGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadFromGameInstance();

	// 委托事件
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUnitSlotChanged OnUnitSlotChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBattleSlotChanged OnBattleSlotChanged;

protected:
	// 总槽位数 (n)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	int32 MaxTotalSlots = 12;

	// 战斗槽位数 (m)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	int32 MaxBattleSlots = 4;

	// 血量恢复成本
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	float HealthRestoreCostPerPercent = 1.0f;

	// 魔法值恢复成本
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	float ManaRestoreCostPerPercent = 0.5f;

	// 所有单位槽位
	UPROPERTY(BlueprintReadOnly, Category = "Unit Data")
	TArray<FUnitSlotData> UnitSlots;

	// 战斗配置槽位
	UPROPERTY(BlueprintReadOnly, Category = "Battle Data")
	TArray<FUnitSlotData> BattleSlots;

	// 已召唤的单位实例（用于跟踪）- 不暴露给蓝图
	TArray<TWeakObjectPtr<AEWUnitBase>> SummonedUnits;

private:
	// 内部工具函数
	bool IsValidSlotIndex(int32 SlotIndex) const;
	bool IsValidBattleSlotIndex(int32 BattleSlotIndex) const;
	int32 FindEmptySlot() const;
	void BroadcastSlotChanged(int32 SlotIndex);
	void BroadcastBattleSlotChanged(int32 BattleSlotIndex);
};
