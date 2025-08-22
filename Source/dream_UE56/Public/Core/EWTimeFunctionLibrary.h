// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EWTimeFunctionLibrary.generated.h"

class UEWTimeManager;
class AEWCharacterBase;

/**
 * 时间控制蓝图函数库
 * 提供蓝图可调用的时间暂停功能
 */
UCLASS()
class DREAM_UE56_API UEWTimeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 获取时间管理器
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	static UEWTimeManager* GetTimeManager(const UObject* WorldContextObject);

	// 暂停游戏时间
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	static bool PauseGameTime(const UObject* WorldContextObject, AEWCharacterBase* Instigator);

	// 恢复游戏时间
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	static void ResumeGameTime(const UObject* WorldContextObject);

	// 检查时间是否被暂停
	UFUNCTION(BlueprintCallable, Category = "Time Control", BlueprintPure)
	static bool IsGameTimePaused(const UObject* WorldContextObject);

	// 获取时间暂停发起者
	UFUNCTION(BlueprintCallable, Category = "Time Control", BlueprintPure)
	static AEWCharacterBase* GetTimePauseInstigator(const UObject* WorldContextObject);

	// 注册Actor为时间敏感
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	static void RegisterTimeSensitiveActor(const UObject* WorldContextObject, AActor* Actor);

	// 注销时间敏感Actor
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	static void UnregisterTimeSensitiveActor(const UObject* WorldContextObject, AActor* Actor);

	// 检查Actor是否免疫时间暂停
	UFUNCTION(BlueprintCallable, Category = "Time Control", BlueprintPure)
	static bool IsActorImmuneToTimePause(const UObject* WorldContextObject, AActor* Actor);

	// 设置Actor的时间暂停免疫状态
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	static void SetActorTimePauseImmune(AActor* Actor, bool bImmune);
};
