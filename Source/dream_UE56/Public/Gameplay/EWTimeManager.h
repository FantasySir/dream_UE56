// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/EngineTypes.h"
#include "EWTimeManager.generated.h"

class AEWCharacterBase;

// 时间暂停委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimePausedChanged, bool, bIsPaused);

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
	void RegisterTimeSensitiveActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Time Management")
	void UnregisterTimeSensitiveActor(AActor* Actor);

	// 检查Actor是否免疫时间暂停
	UFUNCTION(BlueprintCallable, Category = "Time Management")
	bool IsActorImmuneToTimePause(AActor* Actor) const;

	// 委托事件
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTimePausedChanged OnTimePausedChanged;

protected:
	// 时间暂停状态
	UPROPERTY()
	bool bIsTimePaused = false;

	// 时间暂停发起者
	UPROPERTY()
	AEWCharacterBase* TimePauseInstigator = nullptr;

	// 已注册的时间敏感Actor列表
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> TimeSensitiveActors;

	// 免疫时间暂停的Actor列表
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> ImmuneActors;

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

	// 世界Tick回调
	FDelegateHandle TickDelegateHandle;
	void OnWorldTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);
};
