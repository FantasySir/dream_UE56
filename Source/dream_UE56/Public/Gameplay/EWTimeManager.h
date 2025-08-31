// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/EngineTypes.h"
#include "EWTimeManager.generated.h"

class AEWCharacterBase;
class AEWUnitBase;

// 时间暂停委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimePausedChanged, bool, bIsPaused);

// Actor时间暂停管理结构
USTRUCT()
struct FTimePauseActorGroup
{
	GENERATED_BODY()

	// 免疫时间暂停的玩家角色列表
	UPROPERTY()
	TArray<TWeakObjectPtr<AEWCharacterBase>> ImmunePlayers;

	// 敏感时间暂停的单位列表
	UPROPERTY()
	TArray<TWeakObjectPtr<AEWUnitBase>> SensitiveUnits;

	// 免疫时间暂停的单位列表
	UPROPERTY()
	TArray<TWeakObjectPtr<AEWUnitBase>> ImmuneUnits;

	// 敏感时间暂停的其他Actor列表
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> SensitiveActors;

	// 免疫时间暂停的其他Actor列表
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> ImmuneActors;

	// 清理无效引用
	void CleanupInvalidReferences();

	// 获取所有敏感Actor的数量
	int32 GetSensitiveActorCount() const;

	// 获取所有免疫Actor的数量
	int32 GetImmuneActorCount() const;
};

/**
 * 时间管理器子系统
 * 负责管理游戏世界的时间暂停功能，让玩家角色免疫时间暂停效果
 */
UCLASS()
class DREAM_UE56_API UEWTimeManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UEWTimeManager();

	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// 时间暂停控制
	UFUNCTION(BlueprintCallable, Category = "Time Management")
	void PauseTime(AEWCharacterBase* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Time Management")
	void ResumeTime();

	UFUNCTION(BlueprintCallable, Category = "Time Management")
	bool IsTimePaused() const { return bIsTimePaused; }

	// 获取时间暂停发起者
	UFUNCTION(BlueprintCallable, Category = "Time Management")
	AEWCharacterBase* GetTimePauseInstigator() const { return TimePauseInstigator; }

	// 注册/注销需要受时间影响的Actor
	UFUNCTION(BlueprintCallable, Category = "Time Management")
	void RegisterTimeAffectedActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Time Management")
	void UnregisterTimeAffectedActor(AActor* Actor);

	// 处理Actor时间暂停免疫状态变化
	void OnActorTimePauseImmuneStatusChanged(AActor* Actor, bool bIsImmune);

	// 获取统计信息
	UFUNCTION(BlueprintCallable, Category = "Time Management")
	int32 GetTotalSensitiveActorCount() const;

	UFUNCTION(BlueprintCallable, Category = "Time Management")
	int32 GetTotalImmuneActorCount() const;

	// 委托事件
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTimePausedChanged OnTimePausedChanged;

protected:
	// 时间暂停状态
	UPROPERTY()
	bool bIsTimePaused = false;

	// 时间暂停发起者，同一时间内应该只有一个时间暂停发起者
	UPROPERTY()
	AEWCharacterBase* TimePauseInstigator = nullptr;

	// 分类管理的Actor组
	UPROPERTY()
	FTimePauseActorGroup ActorGroups;

	// 保存的原始时间缩放值
	TMap<TWeakObjectPtr<AActor>, float> OriginalTimeDilations;

	// 应用时间暂停到所有注册的Actor
	void ApplyTimePauseToActors();

	// 恢复所有Actor的时间缩放
	void RestoreTimeDilationToActors();

	// 设置Actor的时间缩放
	void SetActorTimeDilation(AActor* Actor, float TimeDilation);

	// 获取Actor的时间缩放
	float GetActorTimeDilation(AActor* Actor) const;

	// 清理无效的弱引用
	void CleanupInvalidReferences();

	// 内部注册函数
	void RegisterCharacterBase(AEWCharacterBase* Character);
	void RegisterOtherActors(AActor* Actor);

	// 世界Tick回调
	FDelegateHandle TickDelegateHandle;
	void OnWorldTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);
};
