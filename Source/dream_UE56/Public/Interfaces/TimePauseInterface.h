// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TimePauseInterface.generated.h"

// 时间暂停免疫状态变化委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimePauseImmuneChanged, AActor*, Actor, bool, bIsImmune);

UINTERFACE(MinimalAPI, BlueprintType)
class UTimePauseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 时间暂停接口
 * 实现此接口的Actor可以响应时间暂停相关事件
 */
class DREAM_UE56_API ITimePauseInterface
{
	GENERATED_BODY()

public:
	// 检查是否免疫时间暂停
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Time Pause")
	bool IsImmuneToTimePause() const;

	// 时间暂停时调用
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Time Pause")
	void OnTimePaused();

	// 时间恢复时调用
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Time Pause")
	void OnTimeResumed();

	// 时间暂停免疫状态发生变化时调用
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Time Pause")
	void OnTimePauseImmuneStatusChanged(bool bIsImmune);

	// Actor即将被销毁时调用，通知TimeManager清理引用
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Time Pause")
	void OnBeforeDestroy();

    virtual FOnTimePauseImmuneChanged& GetOnTimePauseImmuneChanged() = 0;
};
