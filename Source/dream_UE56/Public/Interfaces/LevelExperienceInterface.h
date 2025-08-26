// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LevelExperienceInterface.generated.h"

// 等级变化委托
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32, bool);
// 经验变化委托
DECLARE_MULTICAST_DELEGATE_OneParam(FOnExperienceChanged, int32);

UINTERFACE(MinimalAPI, BlueprintType)
class ULevelExperienceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 等级经验系统接口
 * 提供统一的等级和经验管理功能
 */
class DREAM_UE56_API ILevelExperienceInterface
{
	GENERATED_BODY()

public:
	// ===================== 等级相关接口 =====================
	
	// 获取当前等级
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Level System")
	int32 GetCurrentLevel() const;

	// 设置等级
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Level System") 
	void SetLevel(int32 InLevel);

	// 增加等级
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Level System")
	void AddToLevel(int32 InLevel);

	// ===================== 经验相关接口 =====================
	
	// 获取当前经验
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Experience System")
	int32 GetCurrentExperience() const;

	// 设置经验
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Experience System")
	void SetExperience(int32 InExperience);

	// 增加经验
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Experience System")
	void AddToExperience(int32 InExperience);

	// ===================== C++实现的虚函数版本 =====================
	
	// C++版本的等级相关函数
	virtual int32 GetLevel_Implementation() const { return 1; }
	virtual void SetLevel_Implementation(int32 InLevel) {}
	virtual void AddToLevel_Implementation(int32 InLevel) {}

	// C++版本的经验相关函数
	virtual int32 GetExperience_Implementation() const { return 0; }
	virtual void SetExperience_Implementation(int32 InExperience) {}
	virtual void AddToExperience_Implementation(int32 InExperience) {}

	// ===================== 委托获取 =====================
	
	// 获取等级变化委托
	virtual FOnLevelChanged* GetLevelChangedDelegate() { return nullptr; }
	
	// 获取经验变化委托
	virtual FOnExperienceChanged* GetExperienceChangedDelegate() { return nullptr; }
};
