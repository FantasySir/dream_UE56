// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Character/EWUnitBase.h"
#include "EWAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UEWTimeManager;
class AEWCharacterBase;
struct FGameplayEffectContextHandle;

/**
 * 能力系统蓝图函数库
 * 提供全局静态函数用于能力系统相关操作
 */
UCLASS()
class DREAM_UE56_API UEWAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	 * Ability System Class Defaults
	 */
    UFUNCTION(BlueprintCallable, Category="EWAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes();

	/*
	 * Time Management
	 */
	
	// 获取时间管理器
	UFUNCTION(BlueprintCallable, Category = "EWAbilitySystemLibrary|TimeManagement")
	static UEWTimeManager* GetTimeManager(const UObject* WorldContextObject);
	
	// 检查游戏时间是否被暂停
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EWAbilitySystemLibrary|TimeManagement")
	static bool IsGameTimePaused(const UObject* WorldContextObject);
	
	// 暂停游戏时间
	UFUNCTION(BlueprintCallable, Category = "EWAbilitySystemLibrary|TimeManagement")
	static void PauseGameTime(const UObject* WorldContextObject, AEWCharacterBase* Instigator);
	
	// 恢复游戏时间
	UFUNCTION(BlueprintCallable, Category = "EWAbilitySystemLibrary|TimeManagement")
	static void ResumeGameTime(const UObject* WorldContextObject);

};
