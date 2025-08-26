// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/LevelExperienceInterface.h"
#include "EWPlayerState.generated.h"

class UAbilitySystemComponent;
class UEWBaseAttributeSet;
class UEWPlayerAttributeSet;
class AEWUnitState;

// 单位列表操作类型枚举
UENUM(BlueprintType)
enum class EUnitListOperationType : uint8
{
	AddUnit		UMETA(DisplayName = "Add Unit"),		// 增加单位
	SwapUnit	UMETA(DisplayName = "Swap Unit"),		// 交换单位
	RemoveUnit	UMETA(DisplayName = "Remove Unit"),		// 移除单位
	SetList		UMETA(DisplayName = "Set List")        // 设置列表
};

// 声明委托类型
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUnitListChanged, const TArray<AEWUnitState*>&, UnitList, EUnitListOperationType, OperationType, int32, OperationIndex);

/**
 * 玩家状态类
 * 管理玩家主角的持久化数据，包括玩家专属属性集和统一的单位列表
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API AEWPlayerState : public APlayerState, public IAbilitySystemInterface, public ILevelExperienceInterface
{
	GENERATED_BODY()

public:
	AEWPlayerState();

	// 网络复制
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

	// 获取玩家专属属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System", BlueprintPure)
	UEWPlayerAttributeSet* GetPlayerAttributeSet() const { return PlayerAttributeSet; }

	// 委托声明
	FOnLevelChanged OnLevelChangedDelegate;
	FOnExperienceChanged OnExperienceChangedDelegate;

	// 单位列表管理（只读访问）
	UFUNCTION(BlueprintCallable, Category = "Unit Management", BlueprintPure)
	TArray<AEWUnitState*> GetUnitList() const { return UnitList; }

	// 获取指定索引的单位（安全访问）
	UFUNCTION(BlueprintCallable, Category = "Unit Management", BlueprintPure)
	AEWUnitState* GetUnitAtIndex(int32 Index) const;

	// 获取首发单位列表
	UFUNCTION(BlueprintCallable, Category = "Unit Management", BlueprintPure)
	TArray<AEWUnitState*> GetStartingUnits() const;

	// 获取替补单位列表
	UFUNCTION(BlueprintCallable, Category = "Unit Management", BlueprintPure)
	TArray<AEWUnitState*> GetBenchUnits() const;

	// 单位操作（网络安全）- 仅在服务器端执行
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Unit Management")
	void Server_AddUnit(AEWUnitState* Unit, int32 Index = -1);
	bool Server_AddUnit_Validate(AEWUnitState* Unit, int32 Index);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Unit Management")
	void Server_RemoveUnit(int32 Index);
	bool Server_RemoveUnit_Validate(int32 Index);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Unit Management")
	void Server_SwapUnits(int32 IndexA, int32 IndexB);
	bool Server_SwapUnits_Validate(int32 IndexA, int32 IndexB);

	// 客户端请求函数（便于使用）
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void RequestAddUnit(AEWUnitState* Unit, int32 Index = -1);

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void RequestRemoveUnit(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void RequestSwapUnits(int32 IndexA, int32 IndexB);

	// ===================== 统一的单位操作接口 =====================
	
	// 智能添加单位（自动判断网络环境）
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void AddUnit(AEWUnitState* Unit, int32 Index = -1);

	// 智能移除单位（自动判断网络环境）
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void RemoveUnit(int32 Index);

	// 智能交换单位（自动判断网络环境）
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void SwapUnits(int32 IndexA, int32 IndexB);

	FORCEINLINE int32 GetTotalUnitCount() const { return TotalUnitCount; }
	FORCEINLINE int32 GetStartingUnitCount() const { return StartingUnitCount; }

	// 列表大小管理
	void SetUnitList(const TArray<AEWUnitState*>& NewUnitList);

	// 广播事件
	FOnUnitListChanged OnUnitListChanged;


protected:
	// 能力系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 基础属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UEWBaseAttributeSet> BaseAttributeSet;

	// 玩家专属属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UEWPlayerAttributeSet> PlayerAttributeSet;

	// 统一的单位列表（网络复制）
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Unit Management")
	TArray<AEWUnitState*> UnitList;

	// 总单位数量（网络复制）
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Unit Management")
	int32 TotalUnitCount = 0;

	// 首发单位数量（网络复制）
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Unit Management")
	int32 StartingUnitCount = 0;


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

	// 内部实现函数（直接操作，不走网络）
	void AddUnitInternal(AEWUnitState* Unit, int32 Index = -1);
	void RemoveUnitInternal(int32 Index);
	void SwapUnitsInternal(int32 IndexA, int32 IndexB);


};
